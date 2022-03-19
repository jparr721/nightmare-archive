#pragma once

#include "camera.h"
#include "mesh.h"
#include "opengl.h"

namespace nm {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void bindBuffers();
        void reloadBuffers();
        void resize(integer width, integer height);

    private:
    };
}// namespace nm
