#pragma once

#include "../../math.h"
#include <array>

namespace nm::geometry::primitives {
    auto loadCubeFaces() -> mati;
    auto loadCubeVertices() -> mat;
    void loadSquareGeometry(mat &V, mati &F);
}// namespace nm::geometry::primitives