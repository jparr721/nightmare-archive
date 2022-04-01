#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto massMatrixLinearTetmesh(const matXr &vertices, const matXi &tets, const vecXr &qdot, real density,
                                 const vecXr &tet_volumes) -> spmatXr;
}