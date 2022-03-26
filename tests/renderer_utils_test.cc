#include "../src/renderer_utils.h"
#include <gtest/gtest.h>

constexpr nm::integer kGridScale = 1.0;

TEST(Test_makeRenderableGrid, smallGrid) {
    using namespace nm;

    vecXr vertices;
    vecXr colors;
    makeRenderableGrid(kGridScale, 2, vertices, colors);

    std::vector<std::vector<real>> vertices_compare;
    vertices_compare.push_back({-2, 0, -2});
    vertices_compare.push_back({-2, 0, 2});
    vertices_compare.push_back({-2, 0, -2});
    vertices_compare.push_back({2, 0, -2});
    vertices_compare.push_back({-1, 0, -2});
    vertices_compare.push_back({-1, 0, 2});
    vertices_compare.push_back({-2, 0, -1});
    vertices_compare.push_back({2, 0, -1});
    vertices_compare.push_back({0, 0, -2});
    vertices_compare.push_back({0, 0, 2});
    vertices_compare.push_back({-2, 0, 0});
    vertices_compare.push_back({2, 0, 0});
    vertices_compare.push_back({1, 0, -2});
    vertices_compare.push_back({1, 0, 2});
    vertices_compare.push_back({-2, 0, 1});
    vertices_compare.push_back({2, 0, 1});

    vecXr vertices_compare_vec(48);
    vertices_compare_vec << -2, 0, -2, -2, 0, 2, -2, 0, -2, 2, 0, -2, -1, 0, -2, -1, 0, 2, -2, 0, -1, 2, 0, -1, 0, 0,
            -2, 0, 0, 2, -2, 0, 0, 2, 0, 0, 1, 0, -2, 1, 0, 2, -2, 0, 1, 2, 0, 1;


    EXPECT_EQ(vertices_compare_vec, vertices);
}