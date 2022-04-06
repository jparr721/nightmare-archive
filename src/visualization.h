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

    auto initialize() -> bool;
    auto launch() -> int;
    auto simulationCallback() -> bool;

    // UI Controls
    void callbackDrawViewerMenu();
    auto drawCallback(igl::opengl::glfw::Viewer &viewer) -> bool;
    auto mouseDown(igl::opengl::glfw::Viewer &viewer, int x, int y) -> bool;
    auto mouseUp(igl::opengl::glfw::Viewer &viewer, int x, int y) -> bool;
    auto mouseMove(igl::opengl::glfw::Viewer &viewer, int x, int y) -> bool;
}// namespace nm