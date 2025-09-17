#include "time/time_system.hpp"

namespace marathon {

namespace time {

TimeSystem::TimeSystem()
    : ITimeSystem("marathon.time.time_system"),
        _start(std::chrono::steady_clock::time_point()),
        _lastTick(std::chrono::steady_clock::time_point()) {}

TimeSystem::~TimeSystem() {}

bool TimeSystem::Init() {
    _start = std::chrono::steady_clock::now();
    _lastTick = _start;
    return true;
}

void TimeSystem::Quit() {}

double TimeSystem::Tick() {
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - _lastTick).count();
    _lastTick = now;
    return dt;
}

double TimeSystem::Time() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(now - _start).count();
}

} // namespace time

} // namespace marathon
