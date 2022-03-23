#include "camera.h"
#include "input.h"
#include "shader_program.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


int main(int argc, char **argv) {
    using namespace nm;
    if (!initialize("Nightmare")) { return EXIT_FAILURE; }
    return launch();
}
