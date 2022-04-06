#include "visualization_vars.h"
namespace nm {
    SimulationVars::SimulationVars(real youngsModulus, real poissonsRatio, real density, real dt)
        : youngsModulus(youngsModulus), poissonsRatio(poissonsRatio), density(density), dt(dt) {}

}// namespace nm