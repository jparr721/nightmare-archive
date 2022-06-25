#pragma once

#include "scene.h"

namespace nm::testing::scenes {
    void initializeDropTest(Scene &scene);
    auto simulate() -> bool;
    void launchDropTest(Scene &scene);
}// namespace nm::testing::scenes
