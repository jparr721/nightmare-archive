#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cstddef>
#include <cstdint>

constexpr int kForceSerialThreading = 1;
constexpr int kForceMaxThreadSaturation = 10'000;

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
using triplet = Eigen::Triplet<real>;

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

    /**
     * Computes the matrix identity for the partial derivative of the deformation gradient wrt x.
     * Stolen from Ted Kim's HOBAK. Thanks Ted!
     */
    inline auto partialFpartialx(const mat3 &DmInv) -> mat9x12 {
        const real m = DmInv(0, 0);
        const real n = DmInv(0, 1);
        const real o = DmInv(0, 2);
        const real p = DmInv(1, 0);
        const real q = DmInv(1, 1);
        const real r = DmInv(1, 2);
        const real s = DmInv(2, 0);
        const real t = DmInv(2, 1);
        const real u = DmInv(2, 2);

        const real t1 = -m - p - s;
        const real t2 = -n - q - t;
        const real t3 = -o - r - u;

        mat9x12 PFPu = mat9x12::Zero();
        PFPu(0, 0) = t1;
        PFPu(0, 3) = m;
        PFPu(0, 6) = p;
        PFPu(0, 9) = s;
        PFPu(1, 1) = t1;
        PFPu(1, 4) = m;
        PFPu(1, 7) = p;
        PFPu(1, 10) = s;
        PFPu(2, 2) = t1;
        PFPu(2, 5) = m;
        PFPu(2, 8) = p;
        PFPu(2, 11) = s;
        PFPu(3, 0) = t2;
        PFPu(3, 3) = n;
        PFPu(3, 6) = q;
        PFPu(3, 9) = t;
        PFPu(4, 1) = t2;
        PFPu(4, 4) = n;
        PFPu(4, 7) = q;
        PFPu(4, 10) = t;
        PFPu(5, 2) = t2;
        PFPu(5, 5) = n;
        PFPu(5, 8) = q;
        PFPu(5, 11) = t;
        PFPu(6, 0) = t3;
        PFPu(6, 3) = o;
        PFPu(6, 6) = r;
        PFPu(6, 9) = u;
        PFPu(7, 1) = t3;
        PFPu(7, 4) = o;
        PFPu(7, 7) = r;
        PFPu(7, 10) = u;
        PFPu(8, 2) = t3;
        PFPu(8, 5) = o;
        PFPu(8, 8) = r;
        PFPu(8, 11) = u;

        return PFPu;
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

    inline auto fractalCrossProduct(const mat3 &f0hat, const mat3 &f1hat, const mat3 &f2hat) -> mat9 {
        mat9 HJ = mat9::Zero();
        for (int ii = 0; ii < 9; ++ii) {
            for (int jj = 0; jj < 9; ++jj) {
                HJ(ii + 3, jj) = f2hat(ii, jj);
                HJ(ii, jj + 3) = -f2hat(ii, jj);

                HJ(ii + 6, jj) = -f1hat(ii, jj);
                HJ(ii, jj + 6) = f1hat(ii, jj);

                HJ(ii + 6, jj + 3) = f0hat(ii, jj);
                HJ(ii + 3, jj + 6) = -f0hat(ii, jj);
            }
        }
        return HJ;
    }

    inline auto computeDMatrix(const vec3 &x0, const vec3 &x1, const vec3 &x2, const vec3 &x3) -> mat3 {
        mat3 Ds;
        Ds.col(0) = x1 - x0;
        Ds.col(1) = x2 - x0;
        Ds.col(2) = x3 - x0;
        return Ds;
    }

    inline auto computeF(const mat &V, const mat3 &DmInverse, const vec4i &tet) -> mat3 {
        const vec3 x0 = V.row(tet(0));
        const vec3 x1 = V.row(tet(1));
        const vec3 x2 = V.row(tet(2));
        const vec3 x3 = V.row(tet(3));
        const mat3 Ds = computeDMatrix(x0, x1, x2, x3);
        return Ds * DmInverse;
    }

    inline void buildTwistAndFlipEigenvectors(const mat3 &U, const mat3 &V, mat9 &Q) {
        // create the twist matrices
        mat3 T0, T1, T2;
        T0 << 0, 0, 0, 0, 0, -1, 0, 1, 0;// x-twist
        T1 << 0, 0, 1, 0, 0, 0, -1, 0, 0;// y-twist
        T2 << 0, 1, 0, -1, 0, 0, 0, 0, 0;// z-twist

        const mat3 Q0 = (1.0 / sqrt(2.0)) * (U * T0 * V.transpose());
        const mat3 Q1 = (1.0 / sqrt(2.0)) * (U * T1 * V.transpose());
        const mat3 Q2 = (1.0 / sqrt(2.0)) * (U * T2 * V.transpose());

        // create the flip matrices
        mat3 L0, L1, L2;
        L0 << 0, 0, 0, 0, 0, 1, 0, 1, 0;// x-flip
        L1 << 0, 0, 1, 0, 0, 0, 1, 0, 0;// y-flip
        L2 << 0, 1, 0, 1, 0, 0, 0, 0, 0;// z-flip

        const mat3 Q3 = (1.0 / sqrt(2.0)) * (U * L0 * V.transpose());
        const mat3 Q4 = (1.0 / sqrt(2.0)) * (U * L1 * V.transpose());
        const mat3 Q5 = (1.0 / sqrt(2.0)) * (U * L2 * V.transpose());

        Q.col(0) = vectorize(Q0);
        Q.col(1) = vectorize(Q1);
        Q.col(2) = vectorize(Q2);
        Q.col(3) = vectorize(Q3);
        Q.col(4) = vectorize(Q4);
        Q.col(5) = vectorize(Q5);
    }

    inline void buildScalingEigenvectors(const mat3 &U, const mat3 &Q, const mat3 &V, mat9 &Q9) {
        const vec3 q0 = Q.col(0);
        const vec3 q1 = Q.col(1);
        const vec3 q2 = Q.col(2);

        const mat3 Q0 = U * q0.asDiagonal() * V.transpose();
        const mat3 Q1 = U * q1.asDiagonal() * V.transpose();
        const mat3 Q2 = U * q2.asDiagonal() * V.transpose();

        Q9.col(6) = vectorize(Q0);
        Q9.col(7) = vectorize(Q1);
        Q9.col(8) = vectorize(Q2);
    }
}// namespace utils
