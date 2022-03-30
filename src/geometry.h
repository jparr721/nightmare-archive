#pragma once

#include "nm_math.h"
#include <cmath>

namespace nm {
    auto computeTetrahedralCentroid(const mat43r& tetrahedral_element) -> vec3r;
}// namespace nm