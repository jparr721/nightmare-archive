#include "shader.h"
#include <array>
#include <iostream>

namespace nm {
    Shader::Shader(GLenum type, const char *&shader_src) {
        id = glCreateShader(type);

        assert(id > 0);

        glShaderSource(id, 1, &shader_src, nullptr);
        glCompileShader(id);

        GLint is_compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE) {
            GLint max_length = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

            GLchar info_log[max_length];
            glGetShaderInfoLog(id, max_length, &max_length, info_log);

            // Log this error message
            std::cerr << "Loading Shader Failed: " << info_log << std::endl;

            // Shader compilation failed, nuke this shader from GPU memory
            glDeleteShader(id);
            is_init = false;
        }

        is_init = true;
    }
}// namespace nm