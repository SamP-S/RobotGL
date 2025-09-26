#pragma once

// external
#include "SDL2/SDL_events.h"
#include "imgui.h"

// internal
#include "events/i_event_system.hpp"

namespace marathon {

namespace events {

namespace imgui_sdl2 {

class ImGuiSDL2EventSystem : public IEventSystem {
public:
    ImGuiSDL2EventSystem();
    ~ImGuiSDL2EventSystem() override;

    bool Init() override;
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