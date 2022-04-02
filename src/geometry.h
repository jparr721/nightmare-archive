#pragma once

#include "nm_math.h"
#include <cmath>

namespace nm {
    auto computeTetrahedralCentroid(const mat43r &tetrahedralElement) -> vec3r;
    auto computeTetrahedralCentroid(const mat34r &tetrahedralElement) -> vec3r;
    auto computeTetrahedralElementVolume(const mat43r &tet) -> real;
    auto findMinVertices(const matXr &vertices, real tol) -> std::vector<int>;

}// namespace nm