#include "renderer_utils.h"

namespace nm {
    void makeRenderableGrid(integer spacing_scale, integer uniform_grid_size, vecXr &vertices, vecXr &colors) {
        const vec3r grid_color = vec3r(0.5, 0.5, 0.5);

        matXr vertices_;
        vertices.resize(uniform_grid_size * 2 * 4, 3);
        matXr colors_;
        colors.resize(uniform_grid_size * 2 * 4, 3);
        for (int ii = -uniform_grid_size; ii < uniform_grid_size; ii += 4) {
            vertices_.row(ii) << ii * spacing_scale, 0.0, -uniform_grid_size * spacing_scale;
            vertices_.row(ii + 1) << ii * spacing_scale, 0.0, uniform_grid_size * spacing_scale;
            vertices_.row(ii + 2) << -uniform_grid_size * spacing_scale, 0.0, ii * spacing_scale;
            vertices_.row(ii + 3) << uniform_grid_size * spacing_scale, 0.0, ii * spacing_scale;

            colors_.row(ii) << grid_color;
            colors_.row(ii + 1) << grid_color;
            colors_.row(ii + 2) << grid_color;
            colors_.row(ii + 3) << grid_color;
        }

        vertices = matrixToVector(vertices_);
        colors = matrixToVector(colors_);
    }
}// namespace nm
