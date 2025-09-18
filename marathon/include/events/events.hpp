#pragma once

// internal
#include "core/i_system.hpp"
#include "events/event.hpp"

namespace marathon {

namespace events {

// events API facade
bool Init(BackendFlags flags);
void Quit();
bool PollEvent(Event& e);
bool WaitEvent(Event& e);

/// TODO: Impl for pushing events
/// TODO: Impl for wait until timeout

} // events
    
} //  marathon
