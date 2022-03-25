#include "shader_program.h"

namespace nm {
    ShaderProgram::ShaderProgram() { id = glCreateProgram(); }

    ShaderProgram::~ShaderProgram() {
        std::cout << "Deleting shaders" << std::endl;
        for (const auto &shader : shaders) { glDeleteShader(shader.id); }
        glDeleteProgram(id);
    }

    /**
     * Calls glLinkProgram on the `id` member.
     */
    void ShaderProgram::link() {
        glLinkProgram(id);
        NM_LOG_GL_ERRORS();

        GLint is_linked;
        glGetProgramiv(id, GL_LINK_STATUS, &is_linked);

        if (is_linked == GL_FALSE) {
            int max_length = 1024;
            GLchar info_log[max_length];
            glGetShaderInfoLog(id, max_length, &max_length, info_log);

            // Log this error message
            std::cerr << "Loading Shader Failed: " << info_log << std::endl;
            assert(false && "LINKING SHADER FAILED");
        }
    }

    /**
     * Calls glUseProgram on the `id` member.
     */
    void ShaderProgram::bind() {
        glUseProgram(id);
        NM_LOG_GL_ERRORS();
    }

    /**
     * Unbinds the shader program.
     */
    void ShaderProgram::release() { glUseProgram(0); }

    void ShaderProgram::setMatrixUniform(GLint location, const matXr &uniform) {
        glUniformMatrix4fv(location, 1, GL_FALSE, uniform.data());
    }

    void ShaderProgram::setVectorUniform(GLint location, const vecXr &uniform) {
        glUniform3fv(location, 1, uniform.data());
    }

    void ShaderProgram::addShader(GLenum shader_type, const std::string &shader_path) {
        const auto shader = Shader(shader_type, shader_path);
        assert(shader.is_init);
        shaders.push_back(shader);
        glAttachShader(id, shader.id);
    }

    auto ShaderProgram::uniformLocation(const std::string &name) -> GLint {
        return glGetUniformLocation(id, name.c_str());
    }
}// namespace nm