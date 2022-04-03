#pragma once

#include "mesh.h"
#include "simulation.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <memory>

namespace nm {
    void drawGrid();
    void updateVertexPositions(const vecXr &pos);

    auto viewer() -> igl::opengl::glfw::Viewer &;
    auto menu() -> igl::opengl::glfw::imgui::ImGuiMenu &;
    auto plugin() -> igl::opengl::glfw::imgui::ImGuiPlugin &;
    auto mesh() -> std::unique_ptr<Mesh> &;
    auto simulationState() -> SimulationState &;

    auto initialize() -> bool;
    auto simulationCallback() -> bool;
    auto drawCallback(igl::opengl::glfw::Viewer &viewer) -> bool;
}// namespace nm