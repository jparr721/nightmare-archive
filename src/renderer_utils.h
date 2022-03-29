#include "mesh.h"
#include "nm_math.h"

namespace nm {
    void makeRenderableGrid(real spacing_scale, int uniform_grid_size, matXr &points, matXi &edges,
                            real y_value = 0.0);
}// namespace nm