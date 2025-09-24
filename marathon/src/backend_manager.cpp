#include "backend_manager.hpp"

// std lib
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
    // if (!_valid_impl) {
    //     MT_CORE_CRITICAL("backend_manager.cpp: bm can't init, no valid impl call.");
    //     return false;
    // }
    GetSystem<time::ITimeSystem>(SYS_TIME)->Init();
    GetSystem<window::IWindowSystem>(SYS_WINDOW)->Init();
    GetSystem<events::IEventSystem>(SYS_EVENTS)->Init();
    GetSystem<graphics::IGraphicsSystem>(SYS_GRAPHICS)->Init();
    return true;
}

void BackendManager::Quit() {
    // if (!_valid_impl) {
    //     MT_CORE_CRITICAL("backend_manager.cpp: bm can't init, no valid impl call.");
    //     return;
    // }
    GetSystem<graphics::IGraphicsSystem>(SYS_GRAPHICS)->Quit();
    GetSystem<events::IEventSystem>(SYS_EVENTS)->Quit();
    GetSystem<window::IWindowSystem>(SYS_WINDOW)->Quit();
    GetSystem<time::ITimeSystem>(SYS_TIME)->Quit();
}

template<typename T>
T* BackendManager::GetSystem(SystemID sys) {
    if (!_systems[sys]) {
        MT_CORE_CRITICAL("backend_manager.cpp: no valid impl found for {} system (SYS = {})", typeid(T).name(), (int32_t)sys);
        throw std::runtime_error("Backend Manager has no valid implementation found for requested system.");
    }
    return static_cast<T*>(_systems[sys]);
}

}
