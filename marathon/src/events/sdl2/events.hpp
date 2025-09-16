#pragma once

#include "SDL2/SDL_events.h"

#include "events/events.hpp"


namespace marathon {

namespace events {

namespace sdl2 {

class Events : public marathon::events::Events {
public:
    Events();
    ~Events() override;

    bool Boot() override;
    bool Shutdown() override;

    void Fetch() override;
    void Clear();

private:
    std::shared_ptr<Signal> Convert(const SDL_Event& e);
    std::shared_ptr<Signal> ConvertWindowEvent(const SDL_Event& e);
    std::shared_ptr<Signal> ConvertMouseEvent(const SDL_Event& e);
    std::shared_ptr<Signal> ConvertKeyboardEvent(const SDL_Event& e);
};

} // sdl2

} // events

} // marathon