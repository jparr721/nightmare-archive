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


TEST(Test_computeTetrahedralElementVolume, computesFromTetMatrix) {
    using namespace nm;
    mat43r points;
    points << 0.680375, 0.823295, -0.444451, -0.211234, -0.604897, 0.10794, 0.566198, -0.329554, -0.0452059, 0.59688,
            0.536459, 0.257742;
    real compare = 0.0862827;
    EXPECT_TRUE(computeTetrahedralElementVolume(points) - compare < 0.001);
}