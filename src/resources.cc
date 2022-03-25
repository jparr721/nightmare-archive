#include "resources.h"
#include <fstream>

namespace nm {
    void buildVertexBuffer(GLuint buffer, GLint location, int stride, const vecXr &data) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(real) * data.size(), data.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(location, stride, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(location);
    }

    void buildIndexBuffer(GLuint buffer, const vecXi &data) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(integer) * data.size(), data.data(), GL_STATIC_DRAW);
    }

    auto readShaderFile(const std::string &path) -> std::string {
        std::ifstream stream;
        stream.open(path);
        assert(stream.good() && "FAILED TO READ FILE");
        std::ostringstream file_contents;
        file_contents << stream.rdbuf();
        return file_contents.str();
    }

    auto compileShader(GLenum type, const std::string &shader_path) -> GLuint {
        const auto s_src = readShaderFile(shader_path);
        const char *shader_src = s_src.c_str();

        GLuint shader_id = glCreateShader(type);

        assert(shader_id > 0 && "FAILED TO CREATE SHADER");

        glShaderSource(shader_id, 1, &shader_src, nullptr);
        glCompileShader(shader_id);

        // Check if shader was compiled successfully.
        GLint is_compiled;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

        if (is_compiled == GL_FALSE) {
            GLint max_length = 512;
            GLchar info_log[max_length];

            glGetShaderInfoLog(shader_id, max_length, nullptr, info_log);

            // Log this error message
            std::cerr << "Loading Shader Failed: " << info_log << std::endl;

            assert(false && "COMPILING SHADER FAILED");
        }

        NM_LOG_GL_ERRORS();

        return shader_id;
    }

    auto makeShaderProgram(const std::vector<GLuint> &shader_ids) -> GLuint {
        GLuint shader_program = glCreateProgram();
        for (const auto &shader_id : shader_ids) { glAttachShader(shader_program, shader_id); }
        glLinkProgram(shader_program);

        // Check for linking errors
        GLint is_linked;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE) {
            int max_length = 512;
            GLchar info_log[max_length];
            glGetShaderInfoLog(shader_program, max_length, nullptr, info_log);
            std::cout << "Linking Shader Failed: " << info_log << std::endl;
        }

        for (const auto &shader_id : shader_ids) {
            glDeleteShader(shader_id);
        }

        return shader_program;
    }

    void releaseShaderProgram() { glUseProgram(0); }

    void setMatrixUniform(GLint location, const matXr &uniform) {
        glUniformMatrix4fv(location, 1, GL_FALSE, uniform.data());
    }

    void setVectorUniform(GLint location, const vecXr& uniform) {
        glUniform3fv(location, 1, uniform.data());
    }
}// namespace nm