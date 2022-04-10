#pragma once

#include "fem/V_linear_tetrahedron.h"
#include "fem/assemble.h"
#include "fem/spring_potential.h"
#include "mesh.h"
#include "mouse_control.h"
#include "nm_math.h"
#include "mesh.h"
#include <igl/opengl/glfw/Viewer.h>
#include <vector>

namespace nm {
    auto setupVariables(Mesh &mesh) -> bool;
    auto refreshViewerData() -> vecXr;
}// namespace nm