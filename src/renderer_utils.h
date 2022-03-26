#include "nm_math.h"

namespace nm {
    template<typename T>
    auto flatten(const std::vector<std::vector<T>> &matrix) -> std::vector<T> {
        std::vector<T> out;
        for (const auto &row : matrix) {
            for (const auto &col : row) { out.push_back(col); }
        }
        return out;
    }

    void makeRenderableGrid(real spacing_scale, integer uniform_grid_size, vecXr &vertices, vecXr &colors);
}// namespace nm