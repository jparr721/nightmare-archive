#include "nm_math.h"
#include "simulation.h"
#include "visualization.h"
#include <spdlog/spdlog.h>

constexpr nm::real kDt = 0.01;

bool isSimulating = false;

nm::vecXr q;
nm::vecXr qdot;


auto simulationCallback() -> bool {
    while (isSimulating) { nm::simulate(q, qdot, kDt); }

    return false;
}

auto drawCallback(igl::opengl::glfw::Viewer &viewer) -> bool {
    nm::draw(q);
    return false;
}

auto main() -> int {
#ifdef NDEBUG
    spdlog::info("Logging level is at info");
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::info("Logging level is at debug");
    spdlog::set_level(spdlog::level::debug);
#endif

    assert(nm::viz::initialize());
//    assert(nm::setupVariables(q, qdot, nm::viz::getMeshInstance()));
//    nm::viz::getViewerInstance().callback_post_draw = &drawCallback;
    return nm::viz::getViewerInstance().launch();
}