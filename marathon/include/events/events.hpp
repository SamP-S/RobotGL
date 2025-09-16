#pragma once

// internal
#include "events/event.hpp"
#include "events/i_event_system.hpp"

namespace marathon {

namespace events {

// events API facade
bool Init();
bool Quit();
bool PollEvent(Event& e);
bool WaitEvent(Event& e);

/// TODO: Impl for pushing events
/// TODO: Impl for wait until timeout

} // events
    
} //  marathon
