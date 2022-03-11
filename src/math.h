#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Sparse>
#include <cstdint>
#include <iostream>
#include <vector>

namespace nm {
#ifdef NIGHTMARE_USE_FLOAT
    using real = float;
#else
    using real = double;
#endif

#ifdef NIGHTMARE_USE_32_BIT_INTEGER
    using integer = int64_t;
#else
    using integer = int32_t;
#endif


    // reals
    // Dense vec Types
    using vec2r = Eigen::Matrix<real, 2, 1>;
    using vec3r = Eigen::Matrix<real, 3, 1>;
    using vec4r = Eigen::Matrix<real, 4, 1>;
    using vec6r = Eigen::Matrix<real, 6, 1>;
    using vec12r = Eigen::Matrix<real, 12, 1>;
    using vecXr = Eigen::Matrix<real, Eigen::Dynamic, 1>;

    // Dense Row vec Types
    using Rowvec3r = Eigen::Matrix<real, 1, 3>;
    using RowvecXr = Eigen::Matrix<real, 1, Eigen::Dynamic>;

    // Dense Matrix Types
    using mat2r = Eigen::Matrix<real, 2, 2>;
    using mat3r = Eigen::Matrix<real, 3, 3>;
    using mat4r = Eigen::Matrix<real, 4, 4>;
    using mat6r = Eigen::Matrix<real, 6, 6>;
    using mat36r = Eigen::Matrix<real, 3, 6>;
    using mat12r = Eigen::Matrix<real, 12, 12>;
    using matXr = Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic>;

    // Integers
    // Dense vec Types
    using vec2i = Eigen::Matrix<integer, 2, 1>;
    using vec3i = Eigen::Matrix<integer, 3, 1>;
    using vec4i = Eigen::Matrix<integer, 4, 1>;
    using vec6i = Eigen::Matrix<integer, 6, 1>;
    using vec12i = Eigen::Matrix<integer, 12, 1>;
    using vecXi = Eigen::Matrix<integer, Eigen::Dynamic, 1>;

    // Dense Row vec Types
    using Rowvec3i = Eigen::Matrix<integer, 1, 3>;
    using RowvecXi = Eigen::Matrix<integer, 1, Eigen::Dynamic>;

    // Dense Matrix Types
    using mat2i = Eigen::Matrix<integer, 2, 2>;
    using mat3i = Eigen::Matrix<integer, 3, 3>;
    using mat4i = Eigen::Matrix<integer, 4, 4>;
    using matXi = Eigen::Matrix<integer, Eigen::Dynamic, Eigen::Dynamic>;

    // Sparse MatrixTypes
    using spmatXi = Eigen::SparseMatrix<integer>;

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
    using SparseMatrixX = Eigen::SparseMatrix<T>;

    template<typename Derived>
    inline auto matrixToVector(const Eigen::PlainObjectBase<Derived> &in) -> vecX<typename Derived::Scalar> {
        using Scalar = typename Derived::Scalar;
        const Scalar *data = in.data();
        const auto shape = in.rows() * in.cols();
        return vecX<Scalar>(Eigen::Map<const vecX<Scalar>>(data, shape));
    }

    template<typename T>
    inline auto matrixToVector(const std::vector<std::vector<T>> &in) -> vecX<T> {
        return matrixToVector(stlMatrixToEigenMatrix(in));
    }

    template<typename T>
    inline auto vectorToMatrix(const vecX<T> &in, int rows, int cols) -> matX<T> {
        const T *data = in.data();
        return matX<T>(Eigen::Map<const matX<T>>(data, rows, cols));
    }

    template<typename T>
    inline auto stlMatrixToEigenMatrix(const std::vector<std::vector<T>> &in) -> matX<T> {
        const auto rows = in.size();
        const auto cols = in.at(0).size();

        matX<T> mat(rows, cols);
        for (int ii = 0; ii < rows; ii++) { mat.row(ii) = vecX<T>::Map(&in.at(ii)[0], in.at(ii).size()); }
        return mat;
    }

    template<typename T>
    inline auto stlVectorToEigenVector(const std::vector<T> &in) -> vecX<T> {
        vecX<T> v(in.size());
        for (int row = 0; row < in.size(); ++row) { v(row) = in.at(row); }
        return v;
    }

    template<typename T>
    inline auto eigenVectorToStlVector(const vecX<T> &in) -> std::vector<T> {
        std::vector<T> v;
        v.reserve(in.rows());
        for (int row = 0; row < in.rows(); ++row) { v.push_back(in(row)); }

        return v;
    }

}// namespace nm
