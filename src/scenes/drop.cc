#include "drop.h"
#include <memory>

namespace nm::testing::scenes {
    std::unique_ptr<geometry::TetMesh> squareOne;
    std::unique_ptr<geometry::TetMesh> squareTwo;
    std::unique_ptr<geometry::TetMesh> squareThree;
    std::unique_ptr<geometry::TetMesh> squareFour;
    std::unique_ptr<geometry::TetMesh> bunny;

    void initializeDropTest(Scene &scene) {
        scene.addGrid(0.0, 10.0, 1000);
        mat V;
        mati F;
        // Load square geometry
        geometry::primitives::cube_loadGeometry(V, F);

        // Add the square to each mesh
        squareOne = std::make_unique<geometry::TetMesh>(V, F);
        squareTwo = std::make_unique<geometry::TetMesh>(V, F);
        squareThree = std::make_unique<geometry::TetMesh>(V, F);
        squareFour = std::make_unique<geometry::TetMesh>(V, F);

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
        scene.addShape(squareOne.get());
        scene.addShape(squareTwo.get());
        scene.addShape(squareThree.get());
        scene.addShape(squareFour.get());

        // Add main piece of geometry to drop.
        // bunny = std::unique_ptr<geometry::TetMesh>(scene.addShapeFromFile("bunny.obj"));
    }

    void startDropTest(Scene &scene) {
        // Start simulation background thread

        // Update geometry.

        scene.viewer.launch();
    }
}// namespace nm::testing::scenes

static nm::testing::scenes::Scene scene;

auto main() -> int {
    using namespace nm::testing::scenes;
    nm::testing::scenes::initializeDropTest(scene);
    nm::testing::scenes::startDropTest(scene);
}
