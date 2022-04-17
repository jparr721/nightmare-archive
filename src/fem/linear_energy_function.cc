#include "linear_energy_function.h"
#include <spdlog/spdlog.h>

namespace nm::fem {
    //    auto computeLinearTetrahedralEnergy(const SimulationState &simulationState, const matXr &vertices,
    //                                        const matXi &tets, const vecXr &guess) -> real {
    //        // This is the q + dt * v part of the expression.
    //        const vecXr newq = simulationState.constraint.selectionMatrix.transpose() *
    //                                   (simulationState.q + simulationState.dt * guess) +
    //                           simulationState.constraint.positions;
    //        real energy = 0.0;
    //        for (int ii = 0; ii < tets.rows(); ++ii) {
    //            energy += nm::fem::linearTetrahedronPotentialEnergy(simulationState.getSelectedVertexPositions(), vertices, tets.row(ii),
    //                                                  simulationState.mu, simulationState.lambda,
    //                                                  simulationState.tetVolumes(ii));
    //        }
    //
    //        // Compute the rest of the energy function value.
    //        energy += 0.5 * (guess - simulationState.qdot).transpose() * simulationState.massMatrix *
    //                  (guess - simulationState.qdot);
    //
    //        return energy;
    //    }
    //
    //    auto computeLinearTetrahedralForce(const SimulationState &simulationState, const matXr &vertices, const matXi &tets,
    //                                       const vecXr &guess, std::optional<unsigned int> selectedVertex) -> vecXr {
    //        vecXr forces = -nm::fem::assembleForces(simulationState.getSelectedVertexPositions(), vertices, tets,
    //                                                simulationState.tetVolumes, simulationState.mu, simulationState.lambda);
    //
    //        // Apply forces when we have a selected vertex
    //
    //        return simulationState.massMatrix * (guess - simulationState.qdot) +
    //               simulationState.dt * simulationState.constraint.selectionMatrix * forces;
    //    }
    //
    //    auto computeLinearTetrahedralStiffness(const SimulationState &simulationState, const matXr &vertices,
    //                                           const matXi &tets, const vecXr &guess) -> spmatr {
    //        spmatr stiffness =
    //                -nm::fem::assembleStiffness(simulationState.getSelectedVertexPositions(), vertices, tets,
    //                                            simulationState.tetVolumes, simulationState.mu, simulationState.lambda);
    //        stiffness = simulationState.constraint.selectionMatrix * stiffness *
    //                    simulationState.constraint.selectionMatrix.transpose();
    //        return simulationState.massMatrix + simulationState.dt * simulationState.dt * stiffness;
    //    }
}// namespace nm::fem
