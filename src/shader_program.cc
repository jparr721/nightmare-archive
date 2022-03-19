#include "shader_program.h"

namespace nm {
    ShaderProgram::ShaderProgram() {}

    ShaderProgram::~ShaderProgram() {
        std::cout << "Deleting shaders" << std::endl;
        for (const auto &shader : shaders) { glDeleteShader(shader.id); }
        glDeleteProgram(id);
    }

    void ShaderProgram::link() {
        glLinkProgram(id);
        NM_LOG_GL_ERRORS();
    }

    void ShaderProgram::bind() { glUseProgram(id); }

    void ShaderProgram::release() { glUseProgram(0); }

    void ShaderProgram::setMatrixUniform(GLint location, const matXr &uniform) {
        glUniformMatrix4fv(id, 1, GL_FALSE, uniform.data());
    }

    void ShaderProgram::setVectorUniform(GLint location, const vecXr &uniform) {
        glUniform3fv(location, 1, uniform.data());
    }

    void ShaderProgram::addShader(GLenum shader_type, const char *shader_src) {
        const auto shader = Shader(shader_type, shader_src);
        assert(shader.is_init);
        shaders.push_back(shader);
        glAttachShader(id, shader.id);
    }

    auto ShaderProgram::uniformLocation(const std::string &name) -> GLint {
        return glGetUniformLocation(id, name.c_str());
    }
}// namespace nm