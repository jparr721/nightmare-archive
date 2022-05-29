#pragma once

#include "../../math.h"
#include <array>

namespace nm::geometry::primitives {
    void loadCubeFaces(mati &F);
    void loadCubeVertices(mat &V);
    void loadSquareGeometry(mat &V, mati &F);
}// namespace nm::geometry::primitives