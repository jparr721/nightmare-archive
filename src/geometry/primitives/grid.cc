#include "grid.h"
#include <igl/list_to_matrix.h>
#include <vector>

namespace nm::geometry::primitives {
    void loadGridGeometry(real spacingScale, int gridDimensions, mat &points, mati &edges, real startingy) {
        std::vector<std::vector<real>> points_;
        std::vector<std::vector<int>> edges_;
        for (int ii = -gridDimensions; ii < gridDimensions; ++ii) {
            points_.push_back({ii * spacingScale, startingy, -gridDimensions * spacingScale});
            points_.push_back({ii * spacingScale, startingy, gridDimensions * spacingScale});
            points_.push_back({-gridDimensions * spacingScale, startingy, ii * spacingScale});
            points_.push_back({gridDimensions * spacingScale, startingy, ii * spacingScale});
        }

        for (int ii = 0; ii < points_.size(); ii += 2) {
            edges_.push_back({ii, ii + 1});
            edges_.push_back({ii + 1, ii});
        }

        igl::list_to_matrix(points_, points);
        igl::list_to_matrix(edges_, edges);
    }
}// namespace nm::geometry::primitives