#pragma once

#include <string>
#include <chrono>

#include "core/module.hpp"

namespace marathon {

namespace time {

/// NOTE:
// use steady_clock as it is monotonic
// otherwise the clock jumps around
// it is nanosecond accurate

class Time : public marathon::Module {
protected:
    Time(const std::string& name);
    std::chrono::time_point<std::chrono::steady_clock> _start;
    std::chrono::time_point<std::chrono::steady_clock> _lastTick;
    double _dt;

public:
    ~Time() = default;

    static Time& Instance();

    // module interface
    bool Boot() override;
    bool Shutdown() override;

    /// TODO: 
    // implement averages rather than just the last frame/tick
    // common
    double Tick();
    double GetDeltaTime();
    double GetTime();
    double GetFPS();
};

} // time

} // marathon