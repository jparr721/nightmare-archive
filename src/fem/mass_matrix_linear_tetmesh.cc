#include "mass_matrix_linear_tetmesh.h"
#include "mass_matrix_linear_tetrahedron.h"
#include <igl/volume.h>

namespace nm::fem {
    auto massMatrixLinearTetmesh(const matXr &vertices, const matXi &tets, const vecXr &qdot, real density,
                                 const vecXr &tet_volumes) -> spmatXr {
        using triplet = Eigen::Triplet<real>;
        std::vector<triplet> triplets;
        for (int ii = 0; ii < tets.rows(); ++ii) {
            const mat1212r M = massMatrixLinearTetrahedron(density, tet_volumes(ii));
            const vec4i element = tets.row(ii);
            for (int jj = 0; jj < 4; ++jj) {
                for (int kk = 0; kk < 4; ++kk) {
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 3; ++col) {
                            triplets.emplace_back(3 * element(jj) + row, 3 * element(kk) + col,
                                                  M(3 * jj + row, 3 * kk + col));
                        }
                    }
                }
            }
        }

        spmatXr MM(qdot.rows(), qdot.cols());
        MM.setFromTriplets(triplets.begin(), triplets.end());
        return MM;
    }
}// namespace nm::fem