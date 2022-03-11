#include "../src/renderer_utils.h"
#include <gtest/gtest.h>

constexpr nm::integer kGridScale = 1.0;

TEST(Test_makeRenderableGrid, smallGrid) {
    using namespace nm;

    vecXr vertices;
    vecXr colors;
    makeRenderableGrid(kGridScale, 2, vertices, colors);

    std::cerr << "vertices: " << vertices << std::endl;
    std::cerr << "colors: " << colors << std::endl;

    EXPECT_EQ(true, true);
}
