#include "renderer.h"
#include "renderer_utils.h"
#include "resources.h"
#include <utility>

namespace nm {
    Renderer::Renderer(GLuint shader_program, const std::shared_ptr<Camera> &camera,
                       const RenderMode render_mode)
        : shader_program_(shader_program), camera_(camera), render_mode(render_mode) {
        glUseProgram(shader_program_);
        // Set up vao and vbos
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        bindBuffers();

        // Default lighting
        lighting = Lighting();

        // Configure uniforms
        light_pos_ = glGetUniformLocation(shader_program_, "light");
        view_ = glGetUniformLocation(shader_program_, "view");
        projection_ = glGetUniformLocation(shader_program_, "projection");
        normal_ = glGetUniformLocation(shader_program_, "normal_matrix");

        releaseShaderProgram();
        glEnable(GL_DEPTH_TEST);
    }

    Renderer::~Renderer() {
        for (const auto &buffer_id : buffers) {
            glDeleteBuffers(1, &buffer_id);
            glDeleteVertexArrays(1, &vao);
        }
    }

    void Renderer::bindBuffers() {
        GLuint p_vbo, c_vbo, n_vbo, f_ibo;

        glGenBuffers(1, &p_vbo);
        glGenBuffers(1, &c_vbo);
        glGenBuffers(1, &n_vbo);
        glGenBuffers(1, &f_ibo);

        buffers.at(Buffer::kPosition) = p_vbo;
        buffers.at(Buffer::kColor) = c_vbo;
        buffers.at(Buffer::kNormal) = n_vbo;
        buffers.at(Buffer::kFaces) = f_ibo;

        NM_LOG_GL_ERRORS();
    }

    void Renderer::reloadBuffers(const std::unique_ptr<Mesh> &mesh) {
        buildVertexBuffer(buffers.at(Buffer::kPosition), Buffer::kPosition, 3, mesh->vertices);
        buildVertexBuffer(buffers.at(Buffer::kColor), Buffer::kColor, 3, mesh->colors);

        if (mesh->normals.rows() > 0) {
            buildVertexBuffer(buffers.at(Buffer::kNormal), Buffer::kNormal, 3, mesh->normals);
        }

        buildIndexBuffer(buffers.at(Buffer::kFaces), mesh->faces);
    }

    void Renderer::render(const std::unique_ptr<Mesh> &mesh) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program_);
        setMatrixUniform(view_, camera_->viewMatrix());

        if (mesh != nullptr) {
            reloadBuffers(mesh);

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

        releaseShaderProgram();
    }

    void Renderer::resize(integer width, integer height) {
        std::cout << "Resizing" << std::endl;
        glViewport(0, 0, width, height);
        glUseProgram(shader_program_);
        setMatrixUniform(projection_, camera_->projectionMatrix());
        setMatrixUniform(view_, camera_->viewMatrix());
        setVectorUniform(light_pos_, lighting.light_pos);
        setMatrixUniform(normal_, camera_->viewMatrix().inverse().transpose());
        releaseShaderProgram();
    }

    void Renderer::renderMesh(const std::unique_ptr<Mesh> &mesh) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, mesh->faces.size(), GL_UNSIGNED_INT, nullptr);
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

        buildVertexBuffer(buffers.at(Buffer::kPosition), Buffer::kPosition, 3, vertices);
        buildVertexBuffer(buffers.at(Buffer::kColor), Buffer::kColor, 3, colors);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.rows() / 3));
    }
}// namespace nm