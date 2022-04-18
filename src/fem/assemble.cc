#include "assemble.h"
#include "linear_tetrahedron_potential_energy_gradient_wrt_q.h"
#include "linear_tetrahedron_potential_energy_hessian_wrt_q.h"

namespace nm::fem {

    auto assembleForces(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tetVolumes, real mu,
                        real lambda) -> vecXr {
        vecXr f;
        f.resize(q.rows());
        f.setZero();

        // Compute the force per tet, which is just the gradient of the potential energy with respect to q.
        for (int ii = 0; ii < tets.rows(); ++ii) {
            // Obtain the tetrahedral element force with respect to the reference coordinates
            const vec4i element = tets.row(ii);
            const vec12r dV =
                    linearTetrahedronPotentialEnergyGradientWrtq(q, vertices, element, mu, lambda, tetVolumes(ii));

            // Place into the vector assembly.
            f.segment<3>(3 * tets(ii, 0)) += -dV.segment<3>(0);
            f.segment<3>(3 * tets(ii, 1)) += -dV.segment<3>(3);
            f.segment<3>(3 * tets(ii, 2)) += -dV.segment<3>(6);
            f.segment<3>(3 * tets(ii, 3)) += -dV.segment<3>(9);
        }

        return f;
    }

    auto assembleStiffness(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tetVolumes, real mu,
                           real lambda) -> spmatr {
        using triplet = Eigen::Triplet<real>;
        std::vector<triplet> triplets;
        triplets.reserve(tets.rows() * 4 * 4 * 9);
        for (int ii = 0; ii < tets.rows(); ++ii) {
            const vec4i element = tets.row(ii);
            const mat1212r d2V =
                    linearTetrahedronPotentialEnergyHessianWrtq(q, vertices, element, mu, lambda, tetVolumes(ii));

            for (int jj = 0; jj < 4; ++jj) {
                for (int kk = 0; kk < 4; ++kk) {
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 3; ++col) {
                            const auto i = 3 * tets(ii, jj) + row;
                            const auto j = 3 * tets(ii, kk) + col;
                            const auto v = -d2V(3 * jj + row, 3 * kk + col);
                            triplets.emplace_back(i, j, v);
                        }
                    }
                }
            }
        }

        spmatr stiffnessMatrix(q.rows(), q.rows());
        stiffnessMatrix.setFromTriplets(triplets.begin(), triplets.end());
        return stiffnessMatrix;
    }
}// namespace nm::fem
