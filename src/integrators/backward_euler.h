#pragma once

#include "integrator.h"

namespace nm::integrators {
    void initialize(IntegratorState *integratorState);
    void solveOneTimestep(IntegratorState *integratorState);
}// namespace nm::integrators