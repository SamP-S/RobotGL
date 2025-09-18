#pragma once

// std lib
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

// internal
#include "core/i_system.hpp"

namespace marathon {

    extern const std::array<int, 4> validBackends;

    /** @brief Initialise all modules
        @return Returns the number of modules that failed to boot
    */
    int Init(BackendFlags flags = OPENGL | SDL2);

    /** @brief Quit all modules
        @return number of modules that failed to quit
    */
    int Quit();

} // marathon
