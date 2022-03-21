#pragma once

#include "camera.h"
#include "opengl.h"
#include "renderer.h"
#include "shader_program.h"
#include <memory>
#include <string>

namespace nm {

    class Window {
    public:
        bool is_init = false;

        Window(const std::string &title) : title_(title) {}
        ~Window();

        void initialize();
        auto launch() -> int;

        // Compelx setters ============
        void setBackgroundColor();

    private:
        integer window_width = 600;
        integer window_height = 400;

        const std::string title_;

        vec4r background_color_ = vec4r(0.16, 0.16, 0.16, 0.0);

        GLFWwindow *window_;

        std::shared_ptr<Camera> camera_;
        std::shared_ptr<ShaderProgram> shader_program_;

        std::unique_ptr<Renderer> renderer_;
    };
}// namespace nm