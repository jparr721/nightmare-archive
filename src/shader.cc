#include "resources.h"
#include "shader.h"
#include <array>
#include <iostream>

namespace nm {
    Shader::Shader(GLenum type, const std::string &shader_path) {
        const auto s_shader_src = readShaderFile(shader_path);
        const char *shader_src = s_shader_src.c_str();

        id = glCreateShader(type);

        assert(id > 0);

        glShaderSource(id, 1, &shader_src, nullptr);
        glCompileShader(id);

        GLint is_compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE) {
            GLint max_length = 1024;
            GLchar info_log[max_length];
            glGetShaderInfoLog(id, max_length, &max_length, info_log);

            // Log this error message
            std::cerr << "Loading Shader Failed: " << info_log << std::endl;

            assert(false && "COMPILING SHADER FAILED");
        }

        is_init = true;
    }
}// namespace nm