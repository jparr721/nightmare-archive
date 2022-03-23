#pragma once
#include "opengl.h"

namespace nm {
    struct Shader {
        bool is_init;

        GLuint id = 0;
        GLenum type;

        Shader(GLenum type, const std::string &shader_path);

    private:
        auto readShaderFile(const std::string &path) -> std::string;
    };
}// namespace nm