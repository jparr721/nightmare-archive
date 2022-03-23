#include "renderer.h"
#include "renderer_utils.h"
#include <utility>

namespace nm {
    Renderer::Renderer(std::shared_ptr<ShaderProgram> shader_program, std::shared_ptr<Camera> camera,
                       const RenderMode render_mode)
        : shader_program_(std::move(shader_program)), camera_(std::move(camera)), render_mode(render_mode) {
        shader_program_->bind();

        light_pos_ = Uniform<vec3r>(shader_program_->uniformLocation("light_pos"));
        view_ = Uniform<mat4r>(shader_program_->uniformLocation("view"));
        projection_ = Uniform<mat4r>(shader_program_->uniformLocation("projection"));
        normal_ = Uniform<mat4r>(shader_program_->uniformLocation("normal"));

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        bindBuffers();

        shader_program_->release();

        // Default lighting
        lighting = Lighting();

        glEnable(GL_DEPTH_TEST);
    }

    Renderer::~Renderer() {
        for (const auto &buffer_id : buffers) {
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

        buffers.at(Buffer::kPosition) = p_vbo;
        buffers.at(Buffer::kColor) = c_vbo;
        buffers.at(Buffer::kNormal) = n_vbo;
        buffers.at(Buffer::kFaces) = f_vbo;
        NM_LOG_GL_ERRORS();
    }

    void Renderer::reloadBuffers(const std::unique_ptr<Mesh> &mesh) {
        buildVertexBuffer(Buffer::kPosition, buffers.at(Buffer::kPosition), 3, mesh->vertices);
        buildVertexBuffer(Buffer::kColor, buffers.at(Buffer::kColor), 3, mesh->colors);

        if (mesh->normals.rows() > 0) {
            buildVertexBuffer(Buffer::kNormal, buffers.at(Buffer::kNormal), 3, mesh->normals);
        }

        buildIndexBuffer(buffers.at(Buffer::kFaces), mesh->faces);
    }

    void Renderer::render(const std::unique_ptr<Mesh> &mesh) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_program_->bind();
        shader_program_->setMatrixUniform(view_.id, camera_->viewMatrix());
        renderBaseGrid();

        if (mesh != nullptr) {
            switch (render_mode) {
                case RenderMode::kLines:
                    renderLines(mesh);
                    break;
                case RenderMode::kMesh:
                    renderMesh(mesh);
                    break;
                case RenderMode::kMeshAndLines:
                    renderMeshAndLines(mesh);
                    break;
            }
        }

        shader_program_->release();
    }

    void Renderer::buildVertexBuffer(GLint location, GLint buffer, integer stride, const vecXr &data, integer offset,
                                     bool refresh) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        if (refresh) { glBufferData(GL_ARRAY_BUFFER, sizeof(real) * data.size(), data.data(), GL_DYNAMIC_DRAW); }

        glVertexAttribPointer(location, stride, GL_FLOAT, GL_FALSE, stride * sizeof(real), nullptr);

        glEnableVertexAttribArray(location);
        NM_LOG_GL_ERRORS();
    }

    void Renderer::buildIndexBuffer(GLint buffer, const vecXi &data) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(integer) * data.size(), data.data(), GL_STATIC_DRAW);
        NM_LOG_GL_ERRORS();
    }

    void Renderer::resize(integer width, integer height) {
        glViewport(0, 0, width, height);
        shader_program_->bind();
        shader_program_->setMatrixUniform(projection_.id, camera_->projectionMatrix());
        shader_program_->setMatrixUniform(view_.id, camera_->viewMatrix());
        shader_program_->setVectorUniform(light_pos_.id, lighting.light_pos);
        shader_program_->setMatrixUniform(normal_.id, camera_->viewMatrix().inverse().transpose());
        shader_program_->release();
    }

    void Renderer::renderMesh(const std::unique_ptr<Mesh> &mesh) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, mesh->faces.rows(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::renderLines(const std::unique_ptr<Mesh> &mesh) {
        buildVertexBuffer(Buffer::kColor, buffers.at(Buffer::kColor), 3, mesh->wireframe_colors);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, mesh->faces.rows(), GL_UNSIGNED_INT, nullptr);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void Renderer::renderMeshAndLines(const std::unique_ptr<Mesh> &mesh) {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0, -1.0);
        renderMesh(mesh);
        renderLines(mesh);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }

    void Renderer::renderBaseGrid() {
        vecXr vertices, colors;
        makeRenderableGrid(grid_spacing_scale, uniform_grid_size, vertices, colors);

        buildVertexBuffer(Buffer::kPosition, buffers.at(Buffer::kPosition), 3, vertices);
        buildVertexBuffer(Buffer::kColor, buffers.at(Buffer::kColor), 3, colors);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.rows() / 3));
    }
}// namespace nm