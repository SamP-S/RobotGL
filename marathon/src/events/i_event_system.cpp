#include "events/i_event_system.hpp"

namespace marathon {

namespace events {

IEventSystem::IEventSystem(const std::string& name)
    : ISystem(SYS_EVENTS, {SYS_WINDOW}, name) {}

} // namespace events

} // namespace marathon