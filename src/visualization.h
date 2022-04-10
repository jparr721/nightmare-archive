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
    void setMesh(const std::string &filename, bool tetrahedralize = true);

    auto initialize() -> bool;
    auto launch() -> int;

    auto getMeshInstance() -> Mesh &;
    auto getViewerInstance() -> igl::opengl::glfw::Viewer &;
    auto getFaces(const igl::opengl::glfw::Viewer &viewerInstance) -> matXi;
    auto getVertices(const igl::opengl::glfw::Viewer &viewerInstance) -> matXr;

    // UI Controls
    void callbackDrawViewerMenu();
    auto drawCallback(igl::opengl::glfw::Viewer &viewerInstance) -> bool;
    auto mouseDown(igl::opengl::glfw::Viewer &viewerInstance, int x, int y) -> bool;
    auto mouseUp(igl::opengl::glfw::Viewer &viewerInstance, int x, int y) -> bool;
    auto mouseMove(igl::opengl::glfw::Viewer &viewerInstance, int x, int y) -> bool;
}// namespace nm