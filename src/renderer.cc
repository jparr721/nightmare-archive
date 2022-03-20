#include "renderer.h"
#include <utility>

namespace nm {
    Renderer::Renderer(const std::shared_ptr<ShaderProgram> &shader_program, const std::shared_ptr<Camera> &camera,
                       const RenderMode render_mode)
        : shader_program_(std::move(shader_program)), camera_(std::move(camera)), render_mode(render_mode) {
        shader_program_->bind();
        const auto light_pos = Uniform<vec3r>(shader_program_->uniformLocation("light_pos"));
        const auto view = Uniform<mat4r>(shader_program_->uniformLocation("view"));
        const auto projection = Uniform<mat4r>(shader_program_->uniformLocation("projection"));
        const auto normal = Uniform<mat4r>(shader_program_->uniformLocation("normal"));

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        bindBuffers();
        shader_program_->release();
        reloadBuffers();

        // Default lighting
        lighting = Lighting();

        glEnable(GL_DEPTH_TEST);
    }

    Renderer::~Renderer() {
        for (const auto &[_, buffer_id] : buffers) {
            glDeleteBuffers(1, &buffer_id);
            glDeleteVertexArrays(1, &vao);
        }
    }

    void Renderer::bindBuffers() {
        GLuint p_vbo;
        GLuint c_vbo;
        GLuint n_vbo;
        GLuint f_vbo;

        glGenBuffers(1, &p_vbo);
        glGenBuffers(1, &c_vbo);
        glGenBuffers(1, &n_vbo);
        glGenBuffers(1, &f_vbo);

        buffers["positions"] = p_vbo;
        buffers["colors"] = c_vbo;
        buffers["normals"] = n_vbo;
        buffers["faces"] = f_vbo;
    }

    void Renderer::reloadBuffers() {}

    void Renderer::buildVertexBuffer(GLint location, GLint buffer, integer stride, const vecXr &data, integer offset,
                                     bool refresh) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        if (refresh) { glBufferData(GL_ARRAY_BUFFER, sizeof(real) * data.size(), data.data(), GL_DYNAMIC_DRAW); }

        glVertexAttribPointer(location, stride, GL_FLOAT, GL_FALSE, stride * sizeof(real), nullptr);

        glEnableVertexAttribArray(location);
    }

    void Renderer::buildIndexBuffer(GLint buffer, const vecXi &data) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(integer) * data.size(), data.data(), GL_STATIC_DRAW);
    }

    void Renderer::resize(integer width, integer height) { glViewport(0, 0, width, height); }

    void Renderer::renderMesh(const Mesh &mesh) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, mesh.faces.rows(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::renderLines(const Mesh &mesh) {
        buildVertexBuffer(2, buffers["color"], 3, mesh.wireframe_colors);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, mesh.faces.rows(), GL_UNSIGNED_INT, nullptr);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void Renderer::renderMeshAndLines(const Mesh &mesh) {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0, -1.0);
        renderMesh(mesh);
        renderLines(mesh);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}// namespace nm