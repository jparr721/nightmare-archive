#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto assembleMeshMassMatrix() -> spmatXr;
}