#include "events/i_event_system.hpp"

namespace marathon {

namespace events {

IEventSystem::IEventSystem(const std::string& name)
    : ISystem(SystemID::SYS_EVENTS, name) {}

} // namespace events

} // namespace marathon