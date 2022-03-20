#include "math.h"

namespace nm {
    struct Mesh {
        vecXr vertices;
        vecXr faces;
        vecXr tetrahedra;
        vecXr normals;
        vecXr colors;
        vecXr wireframe_colors;
    };
}// namespace nm
