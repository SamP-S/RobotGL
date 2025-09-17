// std lib
#include <algorithm>
#include <array>

// internal
#include "marathon.hpp"
#include "core/i_system.hpp"

namespace marathon {

    const std::array<int, 4> validBackends = {
        GLFW | OPENGL,
        GLFW | VULKAN,
        SDL2 | OPENGL,
        SDL2 | VULKAN
    };

    int Init(Backends flags) {
        /// TODO: implement relevant backend switching/selecting
        
        return 0;
    }

    int Quit() {
        return 0;
    }

} // marathon