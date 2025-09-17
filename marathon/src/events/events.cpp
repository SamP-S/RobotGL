// internal
#include "events/events.hpp"
#include "events/i_event_system.hpp"
#include "events/sdl2/sdl2_event_system.hpp"

namespace marathon {

namespace events {

namespace {
    static IEventSystem* instance;
}

// events API facade
bool Init() {
    /// TODO: replace with backend enum flags select
    instance = new sdl2::SDL2EventSystem();
    return instance->Init();
}

void Quit() {
    instance->Quit();
    instance = nullptr;
}

bool PollEvent(Event& e) {
    return instance->PollEvent(e);
}

bool WaitEvent(Event& e) {
    return instance->WaitEvent(e);
}

} // events
    
} // marathon