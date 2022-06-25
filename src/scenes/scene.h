#pragma once

#include "../geometry/primitives/cube.h"
#include "../geometry/tet_mesh.h"
#include "../math.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <string>

namespace nm::testing::scenes {
    struct Scene {
        igl::opengl::glfw::Viewer viewer;
        igl::opengl::glfw::imgui::ImGuiPlugin plugin;
        igl::opengl::glfw::imgui::ImGuiMenu menu;

        Scene();
        void addGrid(real startingy, real spacingScale, int size);
        void addShape(const std::shared_ptr<geometry::TetMesh> tetMesh);
        auto addShapeFromFile(const std::string &filename) -> std::shared_ptr<geometry::TetMesh>;
        void updateVertexPositions(const vec &V);
    };
}// namespace nm::testing::scenes
