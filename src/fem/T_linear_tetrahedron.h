#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto massMatrixLinearTetrahedron(const vecXr &qdot, const vecXr &element, real density, real volume) -> mat1212r;
    auto TlinearTetrahedron(const vecXr &qdot, const vecXr &element, real density, real volume) -> real;

}// namespace nm::fem