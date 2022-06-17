#pragma once

#include "../../math.h"

namespace nm::geometry::primitives {
    void grid_loadGeometry(real spacingScale, real startingy, int gridDimensions, mat &points, mati &edges);
}
