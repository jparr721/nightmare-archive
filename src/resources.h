#pragma once

#include "nm_math.h"
#include "opengl.h"
#include <string>

namespace nm {
    void buildVertexBuffer(GLuint buffer, GLint location, int stride, const vecXr &data);
    void buildIndexBuffer(GLuint buffer, const vecXi& data);
    void setMatrixUniform(GLint location, const matXr &uniform);
    void setVectorUniform(GLint location, const vecXr& uniform);
    void releaseShaderProgram();

    auto readShaderFile(const std::string& shader_path) -> std::string;
    auto compileShader(GLenum type, const std::string &shader_path) -> GLuint;
    auto makeShaderProgram(const std::vector<GLuint> &shader_ids) -> GLuint;
}