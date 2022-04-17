#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto massMatrixLinearTetmesh(const matXi &tets, const vecXr &qdot, const vecXr &tetVolumes, real density) -> spmatr;
}