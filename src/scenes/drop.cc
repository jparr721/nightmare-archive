#include "drop.h"
#include "../integrators/backward_euler.h"
#include "../integrators/integrator_state.h"
#include <memory>

static nm::testing::scenes::Scene scene;

namespace nm::testing::scenes {
    bool simulating = true;
    std::shared_ptr<geometry::TetMesh> squareOne;
    std::shared_ptr<geometry::TetMesh> squareTwo;
    std::shared_ptr<geometry::TetMesh> squareThree;
    std::shared_ptr<geometry::TetMesh> squareFour;
    std::shared_ptr<geometry::TetMesh> bunny;
    std::unique_ptr<integrators::IntegratorState> integratorState;

    constexpr real E = 1'000;
    constexpr real v = 0.3;

    void initializeDropTest(Scene &scene) {
        real lambda, mu;
        utils::computeLameParametersFromEandv(E, v, lambda, mu);

        scene.addGrid(0.0, 10.0, 1000);
        mat V;
        mati F;
        // Load square geometry
        geometry::primitives::cube_loadGeometry(V, F);

        // Add the square to each mesh
        squareOne = std::make_shared<geometry::TetMesh>(V, F);
        squareTwo = std::make_shared<geometry::TetMesh>(V, F);
        squareThree = std::make_shared<geometry::TetMesh>(V, F);
        squareFour = std::make_shared<geometry::TetMesh>(V, F);

        // Position Squares
        // TODO(@jparr721) Add rotation
        squareOne->translate(vec3(2, 0, 0));  // Bottom Right
        squareTwo->translate(vec3(3, 2, 0));  // Top Right
        squareThree->translate(vec3(0, 1, 0));// Bottom Left
        squareFour->translate(vec3(0, 3, 0)); // Top Left

        // Rotate the cubes so that way the object can slide off of them
        squareOne->rotate(utils::rad(-10), vec3::UnitZ());
        squareTwo->rotate(utils::rad(-10), vec3::UnitZ());
        squareThree->rotate(utils::rad(10), vec3::UnitZ());
        squareFour->rotate(utils::rad(10), vec3::UnitZ());

        // Add Squares To Scene
        scene.addShape(squareOne);
        scene.addShape(squareTwo);
        scene.addShape(squareThree);
        scene.addShape(squareFour);

        // Add main piece of geometry to drop.
        // Bunny is too big TODO
        // bunny = std::shared_ptr<geometry::TetMesh>(scene.addShapeFromFile("bunny.obj"));
        bunny = std::make_shared<geometry::TetMesh>(V, F);
        bunny->translate(vec3(2, 4, 0));

        scene.addShape(bunny);

        // Make the integrator state with the active tet mesh
        integratorState = std::make_unique<integrators::IntegratorState>(bunny);
        integratorState->kinematicsCollisionObjects.push_back(squareOne);
        integratorState->kinematicsCollisionObjects.push_back(squareTwo);
        integratorState->kinematicsCollisionObjects.push_back(squareThree);
        integratorState->kinematicsCollisionObjects.push_back(squareFour);
        integratorState->lambda = lambda;
        integratorState->mu = mu;
    }

    auto simulate() -> bool {
        while (simulating) { integrators::backwardEuler_solve(integratorState.get()); }
        return false;
    }

    static auto drawCallback(igl::opengl::glfw::Viewer &viewer) -> bool {
        scene.updateVertexPositions(integratorState->position);
        return false;
    }

    void startDropTest(Scene &scene) {
        // Start simulation background thread
        auto simThread = std::thread(simulate);
        simThread.detach();

        // Update geometry.
        scene.viewer.callback_post_draw = &drawCallback;

        // Open up the viewer
        scene.viewer.launch(false /* resizable */, false /* fullscreen */, "Bunny Drop");
    }
}// namespace nm::testing::scenes


auto main() -> int {
    using namespace nm::testing::scenes;
    nm::testing::scenes::initializeDropTest(scene);
    nm::testing::scenes::startDropTest(scene);
}
