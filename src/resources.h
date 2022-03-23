#pragma once

#include "nm_math.h"
#include "opengl.h"

namespace nm {
    void buildVertexBuffer(GLuint buffer, GLint location, int stride, const vecXr &data);
}