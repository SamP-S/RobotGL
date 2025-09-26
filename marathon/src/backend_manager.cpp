#include "backend_manager.hpp"

// std lib
#include <algorithm>
#include <type_traits>

// internal
#include "core/logger.hpp"
#include "graphics/opengl/gl_graphics_system.hpp"
#include "window/sdl2/sdl2_window_system.hpp"
#include "time/chrono/time_system.hpp"
#include "events/sdl2/sdl2_event_system.hpp"

namespace marathon {

BackendManager& BackendManager::Instance() {
    static BackendManager instance;
    return instance;
}

bool BackendManager::Impl(BackendFlags flags) {
    /// TODO: clear backend impl*

    // impl time system
    _systems[SYS_TIME] = new time::chrono::TimeSystem();

    // impl window context
    switch (flags & Backends::WINDOW) {
        case Backends::GLFW:
            MT_CORE_ERROR("backend_manager.cpp: Window context backend = GLFW (Not Implemented).");
            return false;
            break;
        case Backends::SDL2:
            MT_CORE_INFO("backend_manager.cpp: Window context backend = SDL2.");
            _systems[SYS_WINDOW] = new window::sdl2::SDL2WindowSystem();
            _systems[SYS_EVENTS] = new events::sdl2::SDL2EventSystem();
            break;
        default:
            MT_CORE_ERROR("backend_manager.cpp: Window context backend = INVALID.");
            return false;
            break;
    }
    // impl graphics api
    switch (flags & Backends::GRAPHICS) {
        case Backends::OPENGL:
            MT_CORE_INFO("backend_manager.cpp: Graphics API = OpenGL.");
            _systems[SYS_GRAPHICS] = new graphics::gl::GLGraphicsSystem();
            break;
        case Backends::VULKAN:
            MT_CORE_ERROR("backend_manager.cpp: Graphics API = Vulkan (Not Implemented).");
            return false;
            break;
        default:
            MT_CORE_ERROR("backend_manager.cpp: Graphics API = INVALID.");
            return false;
            break;
    }
    // impl gui
    switch (flags & Backends::GUI) {
        case Backends::IMGUI:
            MT_CORE_WARN("backend_manager.cpp: Gui library = ImGui.");
            break;
        default:
            MT_CORE_WARN("backend_manager.cpp: Gui library = No Gui.");
            break;
    }
    return true;
}

bool BackendManager::Init() {
    for (SystemID sys : GetSysOrder()) {
        if (!_systems[sys]->Init()) {
            MT_CORE_CRITICAL("backend_manager.cpp: failed to init {} system (SYS = {})", _systems[sys]->GetName(), (int32_t)sys);
            return false;
        }
        MT_CORE_INFO("backend_manager.cpp: successfully inited {} system (SYS = {})", _systems[sys]->GetName(), (int32_t)sys);
    }
    return true;
}

void BackendManager::Quit() {
    for (SystemID sys : GetSysOrder(true)) {
        _systems[sys]->Quit();
        delete _systems[sys];
        _systems[sys] = nullptr;
    }
}

std::vector<SystemID> BackendManager::GetSysOrder(bool reverse) {
    std::vector<SystemID> base_order;
    // create list of implemented systems
    for (int32_t i = 0; i < SystemID::SYS_MAX_ENUM; i++) {
        if (_systems[i]) {
            base_order.push_back((SystemID)i);
        }
    }

    // build system order based on dependencies
    std::vector<SystemID> sys_order;
    std::vector<bool> visited(SystemID::SYS_MAX_ENUM, false);
    // search through dependencies recursively
    std::function<void(SystemID)> visit = [&](SystemID sys) {
        if (visited[sys])
            return;
        visited[sys] = true;
        if (_systems[sys]) {
            for (SystemID dep : _systems[sys]->GetSystemDeps()) {
                if (_systems[dep]) {
                    visit(dep);
                }
            }
        } else {
            MT_CORE_CRITICAL("backend_manager.cpp: system order generation failed, unimplemented system dependency found (SYS = {})", (int32_t)sys);
            throw std::runtime_error("Backend Manager failed to generate system order due to unimplemented system dependency.");
        }
        sys_order.push_back(sys);
    };

    for (SystemID sys : base_order) {
        visit(sys);
    }

    if (reverse) {
        std::reverse(sys_order.begin(), sys_order.end());
    }
    return sys_order;
}

} // marathon
