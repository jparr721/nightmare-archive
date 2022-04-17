#pragma once

#include "../simulation.h"
#include "assemble.h"
#include "linear_tetrahedron_potential_energy.h"

namespace nm::fem {
    /**
     * Computes the potential energy of active vertex positions for the list of tetrahedra. This functionality
     * is meant to be paired with an optimizer like newton's method.
     * @param simulationState The state of the sim at this timestep.
     * @param vertices The vertices of the simulation.
     * @param tets The tetrahedra of the simulation.
     * @param guess The initial guess from newton's method.
     * @return The total potential energy of the tetrahedral volume.
     */
    //    auto computeLinearTetrahedralEnergy(const SimulationState &simulationState, const matXr &vertices,
    //                                        const matXi &tets, const vecXr &guess) -> real;

    /**
     * Computes the energy function gradient (the force) of the tetrahedral volume.
     * @param simulationState The state of the sim at this timestep.
     * @param vertices The vertices of the simulation.
     * @param tets The tetrahedra of the simulation.
     * @param guess The initial guess from newton's method.
     * @return The force vector.
     */
    //    auto computeLinearTetrahedralForce(const SimulationState &simulationState, const matXr &vertices, const matXi &tets,
    //                                       const vecXr &guess, std::optional<unsigned int> selectedVertex) -> vecXr;

    /**
     * Computes the energy function hessian (the stiffness) of the tetrahedral volume.
     * @param simulationState The state of the sim at this timestep.
     * @param vertices The vertices of the simulation.
     * @param tets The tetrahedra of the simulation.
     * @param guess The initial guess from newton's method.
     * @return The stiffness matrix.
     */
    //    auto computeLinearTetrahedralStiffness(const SimulationState &simulationState, const matXr &vertices,
    //                                           const matXi &tets, const vecXr &guess) -> spmatr;
}// namespace nm::fem