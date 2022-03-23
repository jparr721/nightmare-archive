#pragma once

#include "camera.h"
#include "math.h"
#include "opengl.h"

namespace nm {
    class Input {
    public:
        void handleScrollEvent(real xoffset, real yoffset, const std::shared_ptr<Camera> &camera_);
        void handleMouseButtonPress(int button, int action, int mods, const std::shared_ptr<Camera> &camera_);
        void handleCursorPos(int xpos, int ypos, const std::shared_ptr<Camera> &camera_);

    private:
        bool mouse_pressed = false;

        vec2i current_mouse_pos_ = vec2i::Zero();
        vec2i last_mouse_pos_ = vec2i::Zero();
    };
}// namespace nm