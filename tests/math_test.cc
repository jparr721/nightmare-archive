#include "../src/math.h"
#include <gtest/gtest.h>

TEST(Test_matrixToVector, EigenMatrix) {
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
