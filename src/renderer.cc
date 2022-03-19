#include "renderer.h"

namespace nm {
    void Renderer::bindBuffers() {}
    void Renderer::reloadBuffers() {}
    void Renderer::resize(integer width, integer height) { glViewport(0, 0, width, height); }
}// namespace nm