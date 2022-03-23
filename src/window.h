#pragma once

#include "camera.h"
#include "opengl.h"
#include "renderer.h"
#include "shader_program.h"
#include <memory>
#include <string>

namespace nm {

    void destroy();

    auto initialize(const std::string &window_title) -> bool;

    auto launch() -> int;
}// namespace nm