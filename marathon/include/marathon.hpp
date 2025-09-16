#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <cassert>

// include core modules
#include "core/module.hpp"
#include "core/uuid.hpp"
#include "core/resource.hpp"

#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include "events/events.hpp"
#include "time/time.hpp"
// add audio
// add input


namespace marathon {
   
    /** @brief Initialise all modules
        @return Returns the number of modules that failed to boot
    */
    int Init();

    /** @brief Quit all modules
        @return number of modules that failed to quit
    */
    int Quit();

} // marathon
