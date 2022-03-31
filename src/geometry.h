#pragma once

#include "nm_math.h"
#include <cmath>

namespace nm {
    auto computeTetrahedralCentroid(const mat43r& tetrahedral_element) -> vec3r;
    auto computeTetrahedralCentroid(const mat34r& tetrahedral_element) -> vec3r;
    auto computeTetrahedralElementVolume(const mat43r& tet) -> real;

}// namespace nm