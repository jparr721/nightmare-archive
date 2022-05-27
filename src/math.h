#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>

using real = double;

// Dense vector types
using vec2 = Eigen::Vector2d;
using vec3 = Eigen::Vector3d;
using vec9 = Eigen::Matrix<real, 9, 1>;
using vec12 = Eigen::Matrix<real, 12, 1>;

using vec2i = Eigen::Vector2i;
using vec3i = Eigen::Vector3i;
using vec4i = Eigen::Vector4i;

using vec = Eigen::Matrix<real, Eigen::Dynamic, 1>;

// Dense matrix types
using mat3 = Eigen::Matrix3d;
using mat9 = Eigen::Matrix<real, 9, 9>;
using mat3x12 = Eigen::Matrix<real, 3, 12>;
using mat9x12 = Eigen::Matrix<real, 9, 12>;
using mat12 = Eigen::Matrix<real, 12, 12>;

using mat = Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic>;

// Sparse matrix types
using spmat = Eigen::SparseMatrix<real>;

inline auto doubleContraction(const mat3 &A, const mat3 &B) -> real {
    return A(0, 0) * B(0, 0) + A(1, 1) * B(1, 1) + A(2, 2) * B(2, 2) + A(3, 3) * B(3, 3) + A(4, 4) * B(4, 4) +
           A(5, 5) * B(5, 5);
}
