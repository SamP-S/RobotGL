#pragma once

// external
#include "SDL2/SDL_events.h"

// internal
#include "events/event.hpp"
#include "events/i_event_system.hpp"

namespace marathon {

namespace events {

namespace sdl2 {

class SDL2EventSystem : public IEventSystem {
public:
    SDL2EventSystem();
    ~SDL2EventSystem() override;

    bool Init(BackendFlags flags) override;
    void Quit() override;

    bool PollEvent(Event& e) override;
    bool WaitEvent(Event& e) override;

private:
    bool Convert(const SDL_Event& sdl_e, Event& e);
    bool ConvertWindowEvent(const SDL_Event& sdl_e, Event& e);
    bool ConvertMouseEvent(const SDL_Event& sdl_e, Event& e);
    bool ConvertKeyboardEvent(const SDL_Event& sdl_e, Event& e);
};

} // sdl2

} // events

} // marathon