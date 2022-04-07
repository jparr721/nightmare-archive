#include "visualization.h"
#include "mouse_control.h"
#include "renderer_utils.h"
#include "visualization_menu.h"
#include <spdlog/spdlog.h>
#include <thread>

namespace nm {
    constexpr real kYoungsModulus = 6e5;
    constexpr real kPoissonsRatio = 0.4;
    constexpr real kDensity = 0.1;
    constexpr real kDt = 0.01;

    VisualizationVars visualizationVars;
    MouseInputController mouseInputController;
    SimulationVars simulationVars;

    void drawGrid() {
        spdlog::debug("Drawing grid");
        matXr points;
        matXi edges;
        nm::makeRenderableGrid(10.0, 100, points, edges, 0.0);
        visualizationVars.viewer.data().set_edges(points, edges, Rowvec3r(1.0, 1.0, 1.0));
        visualizationVars.viewer.core().background_color = vec4<float>(0.15, 0.15, 0.15, 1.0);
    }

    void updateVertexPositions(const vecXr &pos) {
        // Update vertex positions
        for (unsigned int ii = 0; ii < visualizationVars.mesh->vertices.rows(); ++ii) {
            visualizationVars.mesh->vertices.row(ii) = pos.segment<3>(3 * ii).transpose();
        }

        visualizationVars.viewer.data_list[0].V = visualizationVars.mesh->vertices;

        // Tell viewer to update
        visualizationVars.viewer.data_list[0].dirty |= igl::opengl::MeshGL::DIRTY_POSITION;
    }

    auto simulationCallback() -> bool {
        while (simulationVars.isSimulating) {
            simulate(simulationVars.simulationState, visualizationVars.mesh->vertices,
                     visualizationVars.mesh->tetrahedra, mouseInputController.selectedVertex);
        }
        return false;
    }

    auto drawCallback(igl::opengl::glfw::Viewer &viewer) -> bool {
        updateVertexPositions(simulationVars.simulationState.constraint.selectionMatrix.transpose() *
                                      simulationVars.simulationState.q +
                              simulationVars.simulationState.constraint.positions);
        return false;
    }

    auto mouseDown(igl::opengl::glfw::Viewer &viewer, int x, int y) -> bool {
        mouseInputController.mouseWindow =
                vec3r(viewer.current_mouse_x, viewer.core().viewport(3) - viewer.current_mouse_y, 0.);

        // Project the ray from the mouse and get the mouse pos in world space.
        igl::unproject(mouseInputController.mouseWindow, viewer.core().view, viewer.core().proj, viewer.core().viewport,
                       mouseInputController.mouseWorld);

        // Compute the vertices in world space nearest to the mouse cursor position.
        mouseInputController.selectedVertex = pickNearestVertex(
                mouseInputController.mouseWindow, viewer.core().view.cast<real>(), viewer.core().proj.cast<real>(),
                viewer.core().viewport.cast<real>(), visualizationVars.mesh->vertices, visualizationVars.mesh->faces,
                mouseInputController.pickingTolerance);

        mouseInputController.isMouseDragging = true;

        return false;
    }

    auto mouseUp(igl::opengl::glfw::Viewer &viewer, int x, int y) -> bool {
        mouseInputController.isMouseDragging = false;
        mouseInputController.lastMouseDragWorld.setZero();
        return false;
    }

    auto mouseMove(igl::opengl::glfw::Viewer &viewer, int x, int y) -> bool {
        mouseInputController.lastMouseDragWindow =
                vec3r(viewer.current_mouse_x, viewer.core().viewport(3) - viewer.current_mouse_y, 0.0) -
                mouseInputController.mouseWindow;
        mouseInputController.mouseWindow =
                vec3r(viewer.current_mouse_x, viewer.core().viewport(3) - viewer.current_mouse_y, 0.0);
        mouseInputController.lastMouseDragWorld -= mouseInputController.mouseWorld;
        igl::unproject(mouseInputController.mouseWindow, viewer.core().view, viewer.core().proj, viewer.core().viewport,
                       mouseInputController.mouseWorld);

        if (mouseInputController.isMouseDragging && !mouseInputController.selectedVertex.has_value()) { return true; }

        return false;
    }

    void callbackDrawViewerMenu() { drawVisualizerMenu(visualizationVars, simulationVars); }

    auto initialize() -> bool {
        spdlog::info("Initializing");
        drawGrid();

        // Load sim vars
        simulationVars.youngsModulus = kYoungsModulus;
        simulationVars.poissonsRatio = kPoissonsRatio;
        simulationVars.density = kDensity;
        simulationVars.dt = kDt;


        spdlog::info("Loading mesh");
        visualizationVars.mesh = std::make_unique<Mesh>("assets/bunny.obj");
        if (!tetrahedralizeMesh(visualizationVars.mesh.get())) {
            spdlog::error("Tetrahedralization failed. Exiting");
            return false;
        }

        simulationVars.simulationState = simulationStateFactory(
                visualizationVars.mesh->vertices, visualizationVars.mesh->tetrahedra, simulationVars.youngsModulus,
                simulationVars.poissonsRatio, simulationVars.dt, simulationVars.density);

        simulationVars.simulationState.setSimulationConstraint(
                simulationConstraintFactory(visualizationVars.mesh->vertices, 0.1));

        auto simThread = std::thread(simulationCallback);
        simThread.detach();

        visualizationVars.plugin.widgets.push_back(&visualizationVars.menu);
        visualizationVars.viewer.plugins.push_back(&visualizationVars.plugin);
        visualizationVars.viewer.callback_post_draw = &drawCallback;
        visualizationVars.viewer.callback_mouse_down = &mouseDown;
        visualizationVars.viewer.callback_mouse_move = &mouseMove;
        visualizationVars.viewer.callback_mouse_up = &mouseUp;
        visualizationVars.menu.callback_draw_viewer_menu = &callbackDrawViewerMenu;
        visualizationVars.viewer.data().set_mesh(visualizationVars.mesh->vertices, visualizationVars.mesh->faces);
        return true;
    }

    auto launch() -> int { return visualizationVars.viewer.launch(); }
}// namespace nm