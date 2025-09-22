#include "time/i_time_system.hpp"

namespace marathon {

namespace time {

ITimeSystem::ITimeSystem(const std::string& name)
    : ISystem(SystemID::SYS_TIME, name) {}

} // namespace events

} // namespace marathon