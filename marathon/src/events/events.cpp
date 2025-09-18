#include "events/events.hpp"

// internal
#include "core/logger.hpp"
#include "events/i_event_system.hpp"
#include "events/sdl2/sdl2_event_system.hpp"

namespace marathon {

namespace events {

namespace {
    static IEventSystem* instance = nullptr;
}

// events API facade
bool Init(BackendFlags flags) {
    if (instance) {
        MT_CORE_WARN("events/events.cpp: Cannot init an already initialised system.");
        return false;
    }

    /// TODO: replace with backend enum flags select
    instance = new sdl2::SDL2EventSystem();
    if (!instance->Init(flags)) {
        MT_CORE_ERROR("events/events.cpp: Failed to init event system.");
        delete instance;
        instance = nullptr;
        return false;
    }

    return true;
}

void Quit() {
    if (!instance) {
        MT_CORE_WARN("events/events.cpp: Cannot quit an uninitialised system.");
        return;
    }

    instance->Quit();
    delete instance;
    instance = nullptr;
}

bool PollEvent(Event& e) {
    if (!instance) {
        MT_CORE_WARN("events/events.cpp: Event system not initialised.");
        return false;
    }
    return instance->PollEvent(e);
}

bool WaitEvent(Event& e) {
    if (!instance) {
        MT_CORE_WARN("events/events.cpp: Event system not initialised.");
        return false;
    }
    return instance->WaitEvent(e);
}

} // events
    
} // marathon