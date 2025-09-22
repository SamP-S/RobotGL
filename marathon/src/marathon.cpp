#include "marathon.hpp"

// std lib
#include <algorithm>
#include <array>

// external
#include "SDL2/SDL.h"

// internal
#include "core/i_system.hpp"
#include "core/logger.hpp"
#include "window/window.hpp"
#include "events/events.hpp"
#include "graphics/graphics.hpp"
#include "time/time.hpp"

namespace marathon {

const std::array<int, 4> validBackends = {
    GLFW | OPENGL,
    GLFW | VULKAN,
    SDL2 | OPENGL,
    SDL2 | VULKAN
};

int Init() {
    int num_sys = 4;

    // Initialize systems with the provided backend flags
    if (std::find(validBackends.begin(), validBackends.end(), flags) == validBackends.end()) {
        MT_CORE_ERROR("marathon.cpp: Invalid backend flags for init.");
        return 4;
    }

    SDL_Init(0);
    
    // inits
    if (window::Init(flags))
        num_sys--;
    if (events::Init(flags))
        num_sys--;
    if (graphics::Init(flags))
        num_sys--;
    if (time::Init(flags))
        num_sys--;
    return num_sys;
}

int Quit() {
    SDL_Quit();
    return 0;
}

} // marathon