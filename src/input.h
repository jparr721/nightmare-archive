#pragma once

#include "camera.h"
#include "nm_math.h"
#include "opengl.h"

namespace nm {
    class Input {
    public:
        void handleScrollEvent(real xoffset, real yoffset, std::shared_ptr<Camera> &camera_);
        void handleMouseButtonPress(GLFWwindow *window, int button, int action, int mods,
                                    std::shared_ptr<Camera> &camera_);
        void handleCursorPos(GLFWwindow *window, int xpos, int ypos, std::shared_ptr<Camera> &camera_);

    private:
        bool mouse_pressed = false;

        vec2i current_mouse_pos_ = vec2i::Zero();
        vec2i last_mouse_pos_ = vec2i::Zero();
    };
}// namespace nm