#include "renderer_utils.h"
#include <igl/list_to_matrix.h>
#include <vector>

namespace nm {
    void makeRenderableGrid(real spacing_scale, int uniform_grid_size, matXr &points, matXi &edges, real y_value) {
        std::vector<std::vector<real>> points_;
        std::vector<std::vector<int>> edges_;
        for (int ii = -uniform_grid_size; ii < uniform_grid_size; ++ii) {
            points_.push_back({ii * spacing_scale, y_value, -uniform_grid_size * spacing_scale});
            points_.push_back({ii * spacing_scale, y_value, uniform_grid_size * spacing_scale});
            points_.push_back({-uniform_grid_size * spacing_scale, y_value, ii * spacing_scale});
            points_.push_back({uniform_grid_size * spacing_scale, y_value, ii * spacing_scale});
        }

        for (int ii = 0; ii < points_.size(); ii += 2) {
            edges_.push_back({ii, ii + 1});
            edges_.push_back({ii + 1, ii});
        }

        igl::list_to_matrix(points_, points);
        igl::list_to_matrix(edges_, edges);
    }
}// namespace nm