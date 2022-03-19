#pragma once
#include "opengl.h"

namespace nm {
    struct Shader {
        bool is_init;

        GLint id;
        GLenum type;

        Shader(GLenum type, const char *&shader_src);
    };
}// namespace nm