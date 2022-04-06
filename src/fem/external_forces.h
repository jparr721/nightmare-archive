#pragma once

#include "../nm_math.h"

namespace nm::fem {
    void applyForce(const vecXr &externalForce, vecXr &forces);
}