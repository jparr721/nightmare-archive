#pragma once

#include "mesh.h"
#include "nm_math.h"
#include "simulation.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <memory>

namespace nm {
    struct MouseInputController {
        bool isMouseDragging = false;

        // Mouse position in the window
        vec3r mouseWindow = vec3r::Zero();

        // Mouse position in world space
        vec3r mouseWorld = vec3r::Zero();

        // Last mouse drag vector
        vec3r lastMouseDragWindow = vec3r::Zero();

        // Last mouse drag vector in world space
        vec3r lastMouseDragWorld = vec3r::Zero();
    };

    struct VisualizationVars {
        igl::opengl::glfw::Viewer viewer;
        igl::opengl::glfw::imgui::ImGuiMenu menu;
        igl::opengl::glfw::imgui::ImGuiPlugin plugin;
        std::unique_ptr<Mesh> mesh;
    };

    struct SimulationVars {
        bool isSimulating = false;

        real youngsModulus;
        real poissonsRatio;
        real density;
        real dt;

        SimulationState simulationState;

        SimulationVars() = default;
        SimulationVars(real youngsModulus, real poissonsRatio, real density, real dt);
    };
}// namespace nm