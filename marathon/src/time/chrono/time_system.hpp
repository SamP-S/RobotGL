#pragma once

// std lib
#include <string>
#include <chrono>

// internal
#include "time/i_time_system.hpp"

namespace marathon {

namespace time {

/// NOTE:
// use steady_clock as it is monotonic
// otherwise the clock jumps around
// it is nanosecond accurate

class TimeSystem : public ITimeSystem {
protected:
    std::chrono::time_point<std::chrono::steady_clock> _start;
    std::chrono::time_point<std::chrono::steady_clock> _lastTick;

public:
    TimeSystem();
    virtual ~TimeSystem();

    // system interface
    bool Init(BackendFlags flags) override;
    void Quit() override;

    // time interface
    double Tick();
    double Time();
};

} // time

} // marathon

