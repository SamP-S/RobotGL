#pragma once

// internal
#include "events/event.hpp"

namespace marathon {

namespace events {

// events API facade
bool Init();
void Quit();
bool PollEvent(Event& e);
bool WaitEvent(Event& e);

/// TODO: Impl for pushing events
/// TODO: Impl for wait until timeout

} // events
    
} //  marathon
