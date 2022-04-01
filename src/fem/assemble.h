#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto assembleForces(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tet_volumes, real mu,
                        real lambda) -> vecXr;
}