#include "resources.h"

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
}// namespace nm