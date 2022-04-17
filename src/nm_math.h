#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Sparse>
#include <cstdint>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

namespace nm {
    using real = double;

    // reals
    // Dense vec Types
    using vec2r = Eigen::Matrix<real, 2, 1>;
    using vec3r = Eigen::Matrix<real, 3, 1>;
    using vec4r = Eigen::Matrix<real, 4, 1>;
    using vec6r = Eigen::Matrix<real, 6, 1>;
    using vec9r = Eigen::Matrix<real, 9, 1>;
    using vec12r = Eigen::Matrix<real, 12, 1>;
    using vecXr = Eigen::Matrix<real, Eigen::Dynamic, 1>;

    // Dense Row vec Types
    using rowvec3r = Eigen::Matrix<real, 1, 3>;
    using rowvecXr = Eigen::Matrix<real, 1, Eigen::Dynamic>;

    // Dense Matrix Types
    using mat2r = Eigen::Matrix<real, 2, 2>;
    using mat3r = Eigen::Matrix<real, 3, 3>;
    using mat4r = Eigen::Matrix<real, 4, 4>;
    using mat34r = Eigen::Matrix<real, 3, 4>;
    using mat43r = Eigen::Matrix<real, 4, 3>;
    using mat99r = Eigen::Matrix<real, 9, 9>;
    using mat912r = Eigen::Matrix<real, 9, 12>;
    using mat1212r = Eigen::Matrix<real, 12, 12>;
    using matXr = Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic>;

    // Sparse Matrix Types
    using spmatr = Eigen::SparseMatrix<real>;

    // ints
    // Dense vec Types
    using vec2i = Eigen::Matrix<int, 2, 1>;
    using vec3i = Eigen::Matrix<int, 3, 1>;
    using vec4i = Eigen::Matrix<int, 4, 1>;
    using vec6i = Eigen::Matrix<int, 6, 1>;
    using vec12i = Eigen::Matrix<int, 12, 1>;
    using vecXi = Eigen::Matrix<int, Eigen::Dynamic, 1>;

    // Dense Matrix Types
    using mat2i = Eigen::Matrix<int, 2, 2>;
    using mat3i = Eigen::Matrix<int, 3, 3>;
    using mat4i = Eigen::Matrix<int, 4, 4>;
    using matXi = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;

    // Sparse MatrixTypes
    using spmati = Eigen::SparseMatrix<int>;

    template<typename T>
    using vecX = Eigen::Matrix<T, Eigen::Dynamic, 1>;
    template<typename T>
    using vec2 = Eigen::Matrix<T, 2, 1>;
    template<typename T>
    using vec3 = Eigen::Matrix<T, 3, 1>;
    template<typename T>
    using vec4 = Eigen::Matrix<T, 4, 1>;
    template<typename T>
    using vec6 = Eigen::Matrix<T, 6, 1>;
    template<typename T>
    using vec12 = Eigen::Matrix<T, 12, 1>;

    template<typename T>
    using matX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    template<typename T>
    using mat2 = Eigen::Matrix<T, 2, 2>;
    template<typename T>
    using mat3 = Eigen::Matrix<T, 3, 3>;
    template<typename T>
    using mat4 = Eigen::Matrix<T, 4, 4>;

    template<typename T>
    using spmat = Eigen::SparseMatrix<T>;

    using tripletr = Eigen::Triplet<real>;

    template<typename T>
    inline auto stlMatrixToEigenMatrix(const std::vector<std::vector<T>> &in) -> matX<T> {
        const auto rows = in.size();
        const auto cols = in.at(0).size();

        matX<T> mat(rows, cols);
        for (int ii = 0; ii < rows; ii++) { mat.row(ii) = vecX<T>::Map(&in.at(ii)[0], in.at(ii).size()); }
        return mat;
    }

    template<typename Derived>
    inline auto matrixToVector(const Eigen::PlainObjectBase<Derived> &in) -> vecX<typename Derived::Scalar> {
        return vecX<typename Derived::Scalar>(Eigen::Map<const vecX<typename Derived::Scalar>>(in.data(), in.size()));
    }

    template<typename T>
    inline auto matrixToVector(const std::vector<std::vector<T>> &in) -> vecX<T> {
        return matrixToVector(stlMatrixToEigenMatrix(in));
    }

    template<typename Derived>
    inline auto vectorToMatrix(const Eigen::PlainObjectBase<Derived> &in, int rows, int cols)
            -> matX<typename Derived::Scalar> {
        using scalar = typename Derived::Scalar;
        const scalar *data = in.data();
        return matX<scalar>(Eigen::Map<const matX<scalar>>(data, rows, cols));
    }

    template<typename T>
    inline auto stlVectorToEigenVector(const std::vector<T> &in) -> vecX<T> {
        vecX<T> v(in.size());
        for (int row = 0; row < in.size(); ++row) { v(row) = in.at(row); }
        return v;
    }

    template<typename Derived>
    inline auto eigenVectorToStlVector(const Eigen::PlainObjectBase<Derived> &in)
            -> std::vector<typename Derived::Scalar> {
        using scalar = typename Derived::Scalar;
        std::vector<scalar> v;
        v.reserve(in.rows());
        for (int row = 0; row < in.rows(); ++row) { v.push_back(in(row)); }

        return v;
    }

    template<typename T>
    inline auto flatten(const std::vector<std::vector<T>> &matrix) -> std::vector<T> {
        std::vector<T> out;
        for (const auto &row : matrix) {
            for (const auto &col : row) { out.push_back(col); }
        }
        return out;
    }

    template<typename T>
    inline void checkSpdMatrix(const T &matrix) {
        spdlog::info("=======CHECKING MATRIX==========");
        Eigen::SelfAdjointEigenSolver<T> eigenSolver(matrix);
        spdlog::info("Checking for all valid eigenvalues");
        const vecXr eigenvalues = eigenSolver.eigenvalues().real();
        bool allPositive = true;
        for (auto jj = 0u; jj < eigenvalues.rows(); ++jj) {
            if (eigenvalues(jj) <= 0) { allPositive = false; }
        }

        if (allPositive) {
            spdlog::info("All Eigenvalues > 0");
        } else {
            spdlog::error("Matrix contains invalid eigenvalues");
        }

        // Check if the matrix is symmetric
        if (!matrix.isApprox(matrix.transpose())) {
            spdlog::error("Matrix is not symmetric");
        } else {
            spdlog::info("Matrix is symmetric.");
        }

        bool allDiagGtZero = true;
        const vecXr diag = matrix.diagonal();
        for (auto jj = 0u; jj < diag.rows(); ++jj) {
            if (diag(jj) == 0) { allDiagGtZero = false; }
        }

        if (allDiagGtZero) {
            spdlog::info("All diagonal values are greater than zero");
        } else {
            spdlog::error("Matrix does not have an all nonzero diagonal");
        }
        spdlog::info("================================");
    }

}// namespace nm