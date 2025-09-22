#pragma once

// std lib
#include <cstdlib>

namespace marathon {

enum Backends : int32_t {
    NONE        = 0x0000,
    // gpu api
    OPENGL      = 0x0001,
    VULKAN      = 0x0002,
    GRAPHICS    = 0x000F,
    // window context
    SDL2        = 0x0010,
    GLFW        = 0x0020,
    WINDOW      = 0x00F0,
    // gui toolkit
    IMGUI       = 0x0100,
    GUI         = 0x0F00,
};

typedef int32_t BackendFlags;

} // marathon
