#pragma once

// std lib
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

namespace marathon {


    enum Backends : int32_t {
        NONE        = 0x0000,
        // gpu api
        OPENGL      = 0x0001,
        VULKAN      = 0x0002,
        RENDERER    = 0x000F,
        // window context
        SDL2        = 0x0010,
        GLFW        = 0x0020,
        WINDOW      = 0x00F0,
        // gui toolkit
        IMGUI       = 0x0100,
        GUI         = 0x0F00,
    };
    typedef int32_t BackendFlags;


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
