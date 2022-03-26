#include "renderer_utils.h"
#include <vector>

namespace nm {
    void makeRenderableGrid(real spacing_scale, integer uniform_grid_size, vecXr &vertices, vecXr &colors) {
        std::vector<std::vector<real>> vertices_;
        std::vector<std::vector<real>> colors_;
        for (int ii = -uniform_grid_size; ii < uniform_grid_size; ++ii) {
            vertices_.push_back({ii * spacing_scale, 0.0, -uniform_grid_size * spacing_scale});
            vertices_.push_back({ii * spacing_scale, 0.0, uniform_grid_size * spacing_scale});
            vertices_.push_back({-uniform_grid_size * spacing_scale, 0.0, ii * spacing_scale});
            vertices_.push_back({uniform_grid_size * spacing_scale, 0.0, ii * spacing_scale});

            colors_.push_back({0.5, 0.5, 0.5});
            colors_.push_back({0.5, 0.5, 0.5});
            colors_.push_back({0.5, 0.5, 0.5});
            colors_.push_back({0.5, 0.5, 0.5});
        }

        vertices = stlVectorToEigenVector(flatten(vertices_));
        colors = stlVectorToEigenVector(flatten(colors_));
    }

    void makeCentralAxes() {
    }
}// namespace nm