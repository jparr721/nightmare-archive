#pragma once

#include "mesh.h"
#include "simulation.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <memory>

namespace nm::viz {
    void drawGrid();
    void updateVertexPositions(const vecXr &pos);
    void setMesh(const std::string &filename, bool tetrahedralize = true);

    auto initialize() -> bool;

    auto getIsSimulating() -> bool &;
    auto getIsMouseDragging() -> bool &;
    auto getMeshInstance() -> Mesh &;
    auto getViewerInstance() -> igl::opengl::glfw::Viewer &;
    auto getFaces(const igl::opengl::glfw::Viewer &viewerInstance) -> matXi;
    auto getVertices(const igl::opengl::glfw::Viewer &viewerInstance) -> matXr;
    auto getPickedVertex() -> int &;
    auto getMouseDragWorld() -> vec3r;

    // UI Controls
    void callbackDrawViewerMenu();
    auto mouseDown(igl::opengl::glfw::Viewer &viewerInstance, int x, int y) -> bool;
    auto mouseUp(igl::opengl::glfw::Viewer &viewerInstance, int x, int y) -> bool;
    auto mouseMove(igl::opengl::glfw::Viewer &viewerInstance, int x, int y) -> bool;
}// namespace nm::viz