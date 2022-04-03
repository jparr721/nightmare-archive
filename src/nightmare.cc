#include "visualization.h"
#include <spdlog/spdlog.h>

auto main() -> int {
#ifdef NDEBUG
    spdlog::info("Logging level is at info");
#else
    spdlog::info("Logging level is at debug");
    spdlog::set_level(spdlog::level::debug);
#endif

    assert(nm::initialize());
    nm::viewer().launch();
}