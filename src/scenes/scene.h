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
        void addGrid(real startingy, int size);
        void addShape(const geometry::TetMesh *tetMesh);

        void updateVertexPositions(const mat &V);

        auto addShapeFromFile(const std::string &filename) -> geometry::TetMesh *;
    };
}// namespace nm::testing::scenes
