#pragma once

#include "camera.h"
#include "mesh.h"

namespace nm {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void bindBuffers();
        void reloadBuffers();
        void resize();

    private:
    };
}// namespace nm
