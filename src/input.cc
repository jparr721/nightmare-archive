#include "input.h"

namespace nm {
    void Input::handleScrollEvent(real xoffset, real yoffset, std::shared_ptr<Camera> &camera_) {
        camera_->zoom(-yoffset);
    }

    void Input::handleMouseButtonPress(GLFWwindow *window, int button, int action, int mods,
                                       std::shared_ptr<Camera> &camera_) {

        camera_->rotating = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        camera_->panning = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    }

    void Input::handleCursorPos(GLFWwindow *window, int xpos, int ypos, std::shared_ptr<Camera> &camera_) {
        current_mouse_pos_ = vec2i(xpos, ypos);
        vec2i dpos = last_mouse_pos_ - current_mouse_pos_;

        if (camera_->panning) { camera_->pan(dpos(0), dpos(1)); }
        if (camera_->rotating) { camera_->rotate(dpos(0), dpos(1)); }
        if (camera_->zooming) { camera_->zoom(-dpos(1)); }
        last_mouse_pos_ = current_mouse_pos_;
    }
}// namespace nm