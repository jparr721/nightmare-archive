#include "assemble.h"
#include "d2V_linear_tetrahedron_dq2.h"
#include "dV_linear_tetrahedron_dq.h"

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
            const vec12r fE = dVlinearTetrahedronDq(q, vertices, element, mu, lambda, tetVolumes(ii));

            // Place into the vector assembly.
            f.segment<3>(3 * tets(ii, 0)) += fE.segment<3>(0);
            f.segment<3>(3 * tets(ii, 1)) += fE.segment<3>(3);
            f.segment<3>(3 * tets(ii, 2)) += fE.segment<3>(6);
            f.segment<3>(3 * tets(ii, 3)) += fE.segment<3>(9);
        }

        return f;
    }

    auto assembleStiffness(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tetVolumes, real mu,
                           real lambda) -> spmatXr {
        using triplet = Eigen::Triplet<real>;
        std::vector<triplet> triplets;
        for (int ii = 0; ii < tets.rows(); ++ii) {
            const vec4i element = tets.row(ii);
            const mat1212r d2V = d2VlinearTetrahedronDq2(q, vertices, element, mu, lambda, tetVolumes(ii));

            for (int jj = 0; jj < 4; ++jj) {
                for (int kk = 0; kk < 4; ++kk) {
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 3; ++col) {
                            triplets.emplace_back(3 * element(jj) + row, 3 * element(kk) + col,
                                                  d2V(3 * jj + row, 3 * kk + col));
                        }
                    }
                }
            }
        }

        spmatXr stiffnessMatrix(q.rows(), q.rows());
        stiffnessMatrix.setFromTriplets(triplets.begin(), triplets.end());
        return stiffnessMatrix;
    }
}// namespace nm::fem
