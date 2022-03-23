#pragma once

#include "camera.h"
#include "mesh.h"
#include "nm_math.h"
#include "opengl.h"
#include "shader_program.h"
#include <array>
#include <memory>

namespace nm {
    struct Lighting {
        real phi;
        real r;

        vec3r light_pos;

        explicit Lighting(real phi = 0.0001, real r = 10.0) : phi(phi), r(r) {
            light_pos = vec3r(std::cos(phi) * r, 5.0, std::sin(phi) * r);
        }
    };

    enum class RenderMode { kMesh = 0, kLines, kMeshAndLines };
    enum Buffer { kPosition = 0, kNormal = 1, kColor = 2, kFaces = 3 };

    class Renderer {
    public:
        GLuint vao;

        real grid_spacing_scale = 1.0;
        integer uniform_grid_size = 10.0;

        RenderMode render_mode;

        Lighting lighting;

        std::array<GLuint, 4> buffers;

        Renderer(std::shared_ptr<ShaderProgram> shader_program, std::shared_ptr<Camera> camera, RenderMode render_mode);
        ~Renderer();

        void bindBuffers();
        void reloadBuffers(const std::unique_ptr<Mesh> &mesh);
        void render(const std::unique_ptr<Mesh> &mesh);

        void resize(integer width, integer height);

    private:
        GLint light_pos_;
        GLint view_;
        GLint projection_;
        GLint normal_;

        std::shared_ptr<ShaderProgram> shader_program_;
        std::shared_ptr<Camera> camera_;

        void renderMesh(const std::unique_ptr<Mesh> &mesh);
        void renderLines(const std::unique_ptr<Mesh> &mesh);
        void renderMeshAndLines(const std::unique_ptr<Mesh> &mesh);

        void renderBaseGrid();
    };
}// namespace nm