#include "time/time.hpp"

namespace marathon {

namespace time {



// module interface
bool Init() {
    _start = std::chrono::steady_clock::now();
    _lastTick = _start;
    _active = true;
    return true;
}
bool Quit() {
    _active = false;
    return true;
}

// common
double Time::Tick() {
    auto now = std::chrono::steady_clock::now();
    _dt = std::chrono::duration<double>(now - _lastTick).count();
    _lastTick = now;
    return _dt;
}
double Time::GetDeltaTime() {
    return _dt;
}
double Time::GetTime() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(now - _start).count();
}
double Time::GetFPS() {
    return 1.0 / _dt;
}

} // time

} // marathon