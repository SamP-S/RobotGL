#pragma once

// std lib
#include <array>

// internal
#include "core/backends.hpp"
#include "events/i_event_system.hpp"
#include "graphics/i_graphics_system.hpp"
#include "time/i_time_system.hpp"
#include "window/i_window_system.hpp"

namespace marathon {

class BackendManager {
public:

    void Init(BackendFlags flags = SDL2 | OPENGL);
    void Quit();
    
    template<typename T>
    T* GetSystem(SystemID sys);

private:
    std::array<ISystem*, SystemID::SYS_MAX_ENUM> _systems = { nullptr };

};

}
