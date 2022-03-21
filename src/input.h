#pramgma once

#include "math.h"

namespace nm {
    class Input {
    public:
    private:
        bool mouse_pressed = false;

        vec2i current_mouse_pos_ = vec2i::Zero();
        vec2i last_mouse_pos_ = vec2i::Zero();
    };
}// namespace nm