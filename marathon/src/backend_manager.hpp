#pragma once

// std lib
#include <array>
#include <unordered_map>
#include <vector>


// internal
#include "core/logger.hpp"
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
    
    // template methods must be defined in header
    template<typename T>
    T* GetSystem(SystemID sys) {
        if (!_systems[sys]) {
            MT_CORE_CRITICAL("backend_manager.cpp: no valid impl found for {} system (SYS = {})", typeid(T).name(), (int32_t)sys);
            throw std::runtime_error("Backend Manager has no valid implementation found for requested system.");
        }
        return static_cast<T*>(_systems[sys]);
    }

private:
    BackendManager() = default;
    ~BackendManager() = default;

    BackendManager(const BackendManager&) = delete;
    BackendManager& operator=(const BackendManager&) = delete;

    std::vector<SystemID> GetSysOrder(bool reverse = false);

    std::array<ISystem*, SystemID::SYS_MAX_ENUM> _systems = { nullptr };
};

}
