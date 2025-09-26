#pragma once

// std lib
#include <array>
#include <unordered_map>
#include <vector>


// internal
#include "core/backends.hpp"
#include "events/i_event_system.hpp"
#include "graphics/i_graphics_system.hpp"
#include "time/i_time_system.hpp"
#include "window/i_window_system.hpp"

namespace marathon {

class BackendManager {
public:
    static BackendManager& Instance();

    bool Impl(BackendFlags flags = SDL2 | OPENGL);
    bool Init();
    void Quit();
    
    template<typename T>
    T* GetSystem(SystemID sys);

private:
    BackendManager() = default;
    ~BackendManager() = default;

    BackendManager(const BackendManager&) = delete;
    BackendManager& operator=(const BackendManager&) = delete;

    std::vector<SystemID> GetSysOrder(bool reverse = false);

    std::array<ISystem*, SystemID::SYS_MAX_ENUM> _systems = { nullptr };
};

}
