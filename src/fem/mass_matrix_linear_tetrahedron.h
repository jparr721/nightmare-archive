#pragma once

#include "../nm_math.h"
namespace nm::fem {
    auto massMatrixLinearTetrahedron(real density, real volume) -> mat1212r;
}