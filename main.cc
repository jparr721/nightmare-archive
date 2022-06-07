#include <spdlog/spdlog.h>

auto main() -> int {
#ifdef NDEBUG
    spdlog::info("Logging level is at info");
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::info("Logging level is at debug");
    spdlog::set_level(spdlog::level::debug);
#endif

    return EXIT_SUCCESS;
}
