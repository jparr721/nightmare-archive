#pragma once
#include "math.h"
#include "opengl.h"
#include "shader.h"
#include <string>
#include <vector>

namespace nm {
    class ShaderProgram {
    public:
        GLint id;
        std::vector<Shader> shaders;

        ShaderProgram();
        ~ShaderProgram();

        void link();
        void bind();
        void release();

        void setMatrixUniform(GLint location, const matXr &uniform);
        void setVectorUniform(GLint location, const vecXr &uniform);
        void addShader(GLenum shader_type, const char *shader_src);

        auto uniformLocation(const std::string &name) -> GLint;
    };
}// namespace nm