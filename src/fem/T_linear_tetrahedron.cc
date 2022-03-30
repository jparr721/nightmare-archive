#include "T_linear_tetrahedron.h"

namespace nm::fem {

    auto massMatrixLinearTetrahedron(real density, real volume) -> mat1212r {
        mat1212r M;
        real mass = density * volume;
        real c0 = (1.0 / 10.0) * mass;
        real c1 = (1.0 / 20.0) * mass;

        M.block(0, 0, 3, 3) = c0 * Eigen::Matrix3d::Identity();
        M.block(0, 3, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(0, 6, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(0, 9, 3, 3) = c1 * Eigen::Matrix3d::Identity();

        M.block(3, 0, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(3, 3, 3, 3) = c0 * Eigen::Matrix3d::Identity();
        M.block(3, 6, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(3, 9, 3, 3) = c1 * Eigen::Matrix3d::Identity();

        M.block(6, 0, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(6, 3, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(6, 6, 3, 3) = c0 * Eigen::Matrix3d::Identity();
        M.block(6, 9, 3, 3) = c1 * Eigen::Matrix3d::Identity();

        M.block(9, 0, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(9, 3, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(9, 6, 3, 3) = c1 * Eigen::Matrix3d::Identity();
        M.block(9, 9, 3, 3) = c0 * Eigen::Matrix3d::Identity();

        return M;
    }

    auto TlinearTetrahedron(const vecXr &qdot, const vecXi &element, real density, real volume) -> real {
        const mat1212r M = massMatrixLinearTetrahedron(density, volume);

        // Index into qdot to find the elements that we want.
        vec12r v;
        v.segment<3>(0) = qdot.segment<3>(element(0) * 3);
        v.segment<3>(3) = qdot.segment<3>(element(1) * 3);
        v.segment<3>(6) = qdot.segment<3>(element(2) * 3);
        v.segment<3>(9) = qdot.segment<3>(element(3) * 3);
        return 0.5 * qdot.transpose() * M * qdot;

    }
}// namespace nm::fem
