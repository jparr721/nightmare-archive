#include "mass_matrix_linear_tetrahedron.h"

namespace nm::fem {
    auto massMatrixLinearTetrahedron(real density, real volume) -> mat1212r {
        mat1212r massMatrix;
        real mass = density * volume;
        real c0 = (1.0 / 10.0) * mass;
        real c1 = (1.0 / 20.0) * mass;

        massMatrix.block(0, 0, 3, 3) = c0 * Eigen::Matrix3d::Identity();
        massMatrix.block(0, 3, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(0, 6, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(0, 9, 3, 3) = c1 * Eigen::Matrix3d::Identity();

        massMatrix.block(3, 0, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(3, 3, 3, 3) = c0 * Eigen::Matrix3d::Identity();
        massMatrix.block(3, 6, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(3, 9, 3, 3) = c1 * Eigen::Matrix3d::Identity();

        massMatrix.block(6, 0, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(6, 3, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(6, 6, 3, 3) = c0 * Eigen::Matrix3d::Identity();
        massMatrix.block(6, 9, 3, 3) = c1 * Eigen::Matrix3d::Identity();

        massMatrix.block(9, 0, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(9, 3, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(9, 6, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        massMatrix.block(9, 9, 3, 3) = c0 * Eigen::Matrix3d::Identity();

        return massMatrix;
    }
}// namespace nm::fem