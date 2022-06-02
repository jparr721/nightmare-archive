#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cstddef>
#include <cstdint>

using real = double;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using usize = std::size_t;


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
using mat3 = Eigen::Matrix<real, 3, 3>;
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
    inline auto partialJpartialF(const mat3 &F) -> mat3 {
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
        const real f0 = F(0, i);
        const real f1 = F(1, i);
        const real f2 = F(2, i);

        crossProductMatrix.row(0) << 0, -f2, f1;
        crossProductMatrix.row(1) << f2, 0, -f0;
        crossProductMatrix.row(2) << -f1, f0, 0;
        return crossProductMatrix;
    }

    inline auto rad(real deg) -> real { return deg * M_PI / 180; }
    inline auto deg(real rad) -> real { return rad * 180 / M_PI; }

    inline auto tetVolume(const std::vector<vec3> &tetVertices) -> real {
        const auto d1 = tetVertices.at(1) - tetVertices.at(0);
        const auto d2 = tetVertices.at(2) - tetVertices.at(0);
        const auto d3 = tetVertices.at(3) - tetVertices.at(0);
        return d3.dot(d1.cross(d2)) / 6.0;
    }

    inline auto isSemiPositiveDefinite(const spmat &A) -> bool {
        Eigen::SelfAdjointEigenSolver<spmat> eigenSolver(A);
        const vec eigenvalues = eigenSolver.eigenvalues().real();
        for (auto jj = 0u; jj < eigenvalues.rows(); ++jj) {
            if (eigenvalues(jj) < 0) { return false; }
        }
        return true;
    }
}// namespace utils
