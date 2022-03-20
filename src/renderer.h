#pragma once

#include "camera.h"
#include "mesh.h"
#include "opengl.h"
#include "shader_program.h"
#include <memory>
#include <unordered_map>

namespace nm {
    struct Lighting {
        real phi;
        real r;

        vec3r light_pos;

        Lighting(real phi = 0.0001, real r = 10.0) : phi(phi), r(r) {
            light_pos = vec3r(std::cos(phi) * r, 5.0, std::sin(phi) * r);
        }
    };

    template<typename T>
    struct Uniform {
        GLint id;
        T data;

        Uniform(GLint id) : id(id) {}
        Uniform(GLint id, const T &data) : id(id), data(data) {}
    };

    enum class RenderMode {
        kMesh = 0,
        kLines,
        kMeshAndLines,
    };

    class Renderer {
    public:
        GLuint vao;
        RenderMode render_mode;

        Lighting lighting;

        std::unordered_map<std::string, GLuint> buffers;

        Renderer(const std::shared_ptr<ShaderProgram> &shader_program, const std::shared_ptr<Camera> &camera,
                 const RenderMode render_mode);
        ~Renderer();

        void bindBuffers();
        void reloadBuffers();
        void render();

        void resize(integer width, integer height);

        // Buffers
        void buildVertexBuffer(GLint location, GLint buffer, integer stride, const vecXr &data, integer offset = 0,
                               bool refresh = true);
        void buildIndexBuffer(GLint buffer, const vecXi &data);

    private:
        std::shared_ptr<ShaderProgram> shader_program_;
        std::shared_ptr<Camera> camera_;

        void renderMesh(const Mesh &mesh);
        void renderLines(const Mesh &mesh);
        void renderMeshAndLines(const Mesh &mesh);
    };
}// namespace nm
