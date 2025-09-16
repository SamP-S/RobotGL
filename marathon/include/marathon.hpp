#pragma once

// std lib
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

namespace marathon {

    enum Backends {
        DEFAULT = 0x00,
        OPENGL  = 0x01,
        VULKAN  = 0x01 << 1,
        SDL2    = 0x01 << 2,
        GLFW    = 0x01 << 3,
        NONE    = 0x01 << 4
    };

    extern const std::array<int, 4> validBackends;

    /** @brief Initialise all modules
        @return Returns the number of modules that failed to boot
    */
    int Init(Backends flags = Backends::DEFAULT);

    /** @brief Quit all modules
        @return number of modules that failed to quit
    */
    int Quit();

} // marathon
