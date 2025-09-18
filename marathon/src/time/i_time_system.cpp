#include "time/i_time_system.hpp"

namespace marathon {

namespace time {

ITimeSystem::ITimeSystem(const std::string& name)
    : ISystem(SystemType::SYS_TIME, name) {}

} // namespace events

} // namespace marathon