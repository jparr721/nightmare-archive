#pragma once

#include "nm_math.h"
#include "opengl.h"
#include <string>

namespace nm {
    void buildVertexBuffer(GLuint buffer, GLint location, int stride, const vecXr &data);
    void buildIndexBuffer(GLuint buffer, const vecXi& data);
    auto readShaderFile(const std::string& shader_path) -> std::string;
}