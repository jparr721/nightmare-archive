#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto linearTetrahedronPotentialEnergyGradientWrtq(const vecXr &q, const matXr &vertices, const vec4i &element,
                                                      real mu, real lambda, real volume) -> vec12r;
}