#include "time/i_time_system.hpp"

namespace marathon {

namespace time {

ITimeSystem::ITimeSystem(const std::string& name)
    : ISystem(SYS_TIME, {}, name) {}

} // namespace time

} // namespace marathon