#include "drop.h"

namespace nm::testing::scenes {
    void initializeDropTest(Scene &scene) { scene.viewer.launch(); }
}// namespace nm::testing::scenes

static nm::testing::scenes::Scene scene;

auto main() -> int {
    using namespace nm::testing::scenes;
    nm::testing::scenes::initializeDropTest(scene);
}
