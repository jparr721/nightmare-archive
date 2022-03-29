#include "../src/renderer_utils.h"
#include <gtest/gtest.h>

constexpr int kGridScale = 1.0;

TEST(Test_makeRenderableGrid, smallGrid) {
    using namespace nm;

    rowMatXr vertices;
    rowMatXr colors;
    makeRenderableGrid(kGridScale, 2, 0.0);

    rowMatXr vertices_compare(16, 3);
    vertices_compare << -2, 0, -2, -2, 0, 2, -2, 0, -2, 2, 0, -2, -1, 0, -2, -1, 0, 2, -2, 0, -1, 2, 0, -1, 0, 0,
            -2, 0, 0, 2, -2, 0, 0, 2, 0, 0, 1, 0, -2, 1, 0, 2, -2, 0, 1, 2, 0, 1;


    EXPECT_EQ(vertices_compare, vertices);
}