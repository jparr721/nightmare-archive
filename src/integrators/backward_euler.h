#pragma once

#include "integrator_state.h"

namespace nm::integrators {
    void backwardEuler_solve(IntegratorState *integratorState);
}// namespace nm::integrators
