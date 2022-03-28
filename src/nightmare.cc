#include "visualization.h"

auto main() -> int {
    assert(nm::initialize());
    nm::viewer().launch();
}