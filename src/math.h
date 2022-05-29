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
using mati = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;

// Sparse matrix types
using spmat = Eigen::SparseMatrix<real>;

inline auto doubleContraction(const mat3 &A, const mat3 &B) -> real {
    return A(0, 0) * B(0, 0) + A(1, 1) * B(1, 1) + A(2, 2) * B(2, 2) + A(3, 3) * B(3, 3) + A(4, 4) * B(4, 4) +
           A(5, 5) * B(5, 5);
}

namespace identities {
    /**
     * Computes the matrix identity for the partial derivative of J wrt F
     */
    inline auto gradientJ(const mat3 &F) -> mat3 {
        mat3 gradJ;
        gradJ.col(0) = F.col(1).cross(F.col(2));
        gradJ.col(1) = F.col(2).cross(F.col(0));
        gradJ.col(2) = F.col(0).cross(F.col(1));
        return gradJ;
    }
}// namespace identities

namespace utils {
    /**
     * Colwise vectorization of a matrix into a vector for proper shaping
     * during hessian calculations
     */
    inline auto vectorize(const mat3 &A) -> vec9 {
        vec9 ret;
        ret(0) = A(0, 0);
        ret(1) = A(0, 1);
        ret(2) = A(0, 2);
        ret(3) = A(1, 1);
        ret(4) = A(1, 2);
        ret(5) = A(1, 0);
        ret(6) = A(2, 0);
        ret(7) = A(2, 1);
        ret(8) = A(2, 2);
        return ret;
    }

    inline auto crossMatrix(const mat3 &F, int i) -> mat3 {
        mat3 crossProductMatrix;
        const auto f0 = F(0, i);
        const auto f1 = F(1, i);
        const auto f2 = F(2, i);

        crossProductMatrix.row(0) << 0, -f2, f1;
        crossProductMatrix.row(1) << f2, 0, f1;
        crossProductMatrix.row(2) << -f1, f0, 0;
        return crossProductMatrix;
    }

}// namespace utils
