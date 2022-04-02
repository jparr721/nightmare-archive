#include "renderer_utils.h"
#include <igl/list_to_matrix.h>
#include <vector>

namespace nm {
    void makeRenderableGrid(real spacingScale, int uniformGridSize, matXr &points, matXi &edges, real yValue) {
        std::vector<std::vector<real>> points_;
        std::vector<std::vector<int>> edges_;
        for (int ii = -uniformGridSize; ii < uniformGridSize; ++ii) {
            points_.push_back({ii * spacingScale, yValue, -uniformGridSize * spacingScale});
            points_.push_back({ii * spacingScale, yValue, uniformGridSize * spacingScale});
            points_.push_back({-uniformGridSize * spacingScale, yValue, ii * spacingScale});
            points_.push_back({uniformGridSize * spacingScale, yValue, ii * spacingScale});
        }

        for (int ii = 0; ii < points_.size(); ii += 2) {
            edges_.push_back({ii, ii + 1});
            edges_.push_back({ii + 1, ii});
        }

        igl::list_to_matrix(points_, points);
        igl::list_to_matrix(edges_, edges);
    }
}// namespace nm