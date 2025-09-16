#pragma once

// internal
#include "events/event.hpp"
#include "events/i_event_system.hpp"

namespace marathon {

namespace events {

// events API facade
bool Init(std::unique_ptr<IEventSystem> impl);
bool Quit();
bool PollEvent(Event& e);
bool WaitEvent(Event& e);

/// TODO: Allow for pushing events
/// TODO: Allow for wait until timeout

} // events
    
} //  marathon
