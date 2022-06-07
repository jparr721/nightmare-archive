#pragma once

#include "../../math.h"

namespace nm::geometry::primitives {
    void loadGridGeometry(real spacingScale, real startingy, int gridDimensions, mat &points, mati &edges);
}