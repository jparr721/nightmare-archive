#pragma once

#include "mesh.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <memory>

namespace nm {
    auto viewer() -> igl::opengl::glfw::Viewer &;
    auto menu() -> igl::opengl::glfw::imgui::ImGuiMenu &;
    auto plugin() -> igl::opengl::glfw::imgui::ImGuiPlugin &;
    auto mesh() -> std::unique_ptr<Mesh> &;

    auto initialize() -> bool;

}// namespace nm