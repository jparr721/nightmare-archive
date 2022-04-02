#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto assembleForces(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tetVolumes, real mu,
                        real lambda) -> vecXr;

    auto assembleStiffness(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tetVolumes, real mu,
                           real lambda) -> spmatXr;
}// namespace nm::fem