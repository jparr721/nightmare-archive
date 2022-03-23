#include "../src/nm_math.h"
#include <gtest/gtest.h>

TEST(Test_matrixToVector, doesConvert) {
    using namespace nm;

    mat2r m;
    m << 1, 2, 3, 4;

    vec4r v;
    v << 1, 3, 2, 4;

    const auto compare = matrixToVector(m);


    std::vector<std::vector<real>> m2;
    m2.push_back({1, 2});
    m2.push_back({3, 4});
    const vecXr compare2 = matrixToVector(m2);

    EXPECT_EQ(v, compare);
    EXPECT_EQ(v, compare2);
}

TEST(Test_vectorToMatrix, doesConvert) {
    using namespace nm;

    mat2r m;
    m << 1, 2, 3, 4;

    vec4r v;
    v << 1, 3, 2, 4;

    mat2r mv = nm::vectorToMatrix(v, 2, 2);

    EXPECT_EQ(m, mv);
}

TEST(Test_eigenVectorToStlVector, doesConvert) { using namespace nm;

    vec4r v;
    v << 1, 2, 3, 4;

    const auto ov = eigenVectorToStlVector(v);

    EXPECT_EQ(ov.at(0), 1);
    EXPECT_EQ(ov.at(1), 2);
    EXPECT_EQ(ov.at(2), 3);
    EXPECT_EQ(ov.at(3), 4);
}
