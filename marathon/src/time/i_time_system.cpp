// internal
#include "time/i_time_system.hpp"

namespace marathon {

namespace time {

ITimeSystem::ITimeSystem(const std::string& name)
    : ISystem(SystemType::TIME, name) {}

} // namespace events

} // namespace marathon