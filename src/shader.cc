#include "shader.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

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

            // Shader compilation failed, nuke this shader from GPU memory
            glDeleteShader(id);

            assert(false && "COMPILING SHADER FAILED");
        }

        is_init = true;
    }

    auto Shader::readShaderFile(const std::string &path) -> std::string {
        std::ifstream stream;
        stream.open(path);
        assert(stream.good() && "FAILED TO READ FILE");
        std::ostringstream file_contents;
        file_contents << stream.rdbuf();
        return file_contents.str();
    }
}// namespace nm