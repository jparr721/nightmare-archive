#include "../src/geometry.h"
#include <gtest/gtest.h>

TEST(Test_computeTetrahedralCentroid, computesRowwise) {
    using namespace nm;
    mat43r input;
    input << 5, -7, 0, 1, 5, 3, 4, -6, 3, 6, -4, 2;
    const vec3r centroid = computeTetrahedralCentroid(input);
    const vec3r centroid_compare(4, -3, 2);
    EXPECT_EQ(centroid, centroid_compare);
}

TEST(Test_computeTetrahedralCentroid, computesColwise) {
    using namespace nm;
    mat43r _input;
    _input << 5, -7, 0, 1, 5, 3, 4, -6, 3, 6, -4, 2;
    mat34r input = _input.transpose();
    const vec3r centroid = computeTetrahedralCentroid(input);
    const vec3r centroid_compare(4, -3, 2);
    EXPECT_EQ(centroid, centroid_compare);
}
