#include "../src/geometry.h"
#include <gtest/gtest.h>

TEST(Test_computeTetrahedralCentroid, doesCompute) {
    using namespace nm;
    mat43r input;
    input << 5, -7, 0, 1, 5, 3, 4, -6, 3, 6, -4, 2;
    const vec3r centroid = computeTetrahedralCentroid(input);
    const vec3r centroid_compare(4, -3, 2);
    EXPECT_EQ(centroid, centroid_compare);
}