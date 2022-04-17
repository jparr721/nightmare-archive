#include "mass_matrix_linear_tetmesh.h"
#include "mass_matrix_linear_tetrahedron.h"

namespace nm::fem {
    auto massMatrixLinearTetmesh(const matXi &tets, const vecXr &qdot, const vecXr &tetVolumes, real density)
            -> spmatr {
        using triplet = Eigen::Triplet<real>;
        std::vector<triplet> triplets;
        triplets.reserve(tets.rows() * 12 * 12);
        for (int ii = 0; ii < tets.rows(); ++ii) {
            const mat1212r massMatrix = massMatrixLinearTetrahedron(density, tetVolumes(ii));
            const vec4i element = tets.row(ii);
            for (int jj = 0; jj < 4; ++jj) {
                for (int kk = 0; kk < 4; ++kk) {
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 3; ++col) {
                            const auto i = 3 * tets(ii, jj) + row;
                            const auto j = 3 * tets(ii, jj) + col;
                            const auto v = massMatrix(3 * jj + row, 3 * kk + col);
                            triplets.emplace_back(i, j, v);
                        }
                    }
                }
            }
        }

        spmatr massMatrix(qdot.rows(), qdot.rows());
        massMatrix.setFromTriplets(triplets.begin(), triplets.end());

        checkSpdMatrix(massMatrix);

        return massMatrix;
    }
}// namespace nm::fem