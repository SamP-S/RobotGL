#pragma once

// std lib
#include <string>
#include <chrono>

// internal
#include "core/i_system.hpp"

namespace marathon {

namespace time {

class ITimeSystem : public ISystem {
public:
    ITimeSystem(const std::string& name);
    virtual ~ITimeSystem();

    // system interface
    bool Init() override;
    bool Quit() override;

    // time interface
    double Tick();
    double GetDeltaTime();
    double GetTime();
    double GetFPS();
};

} // time

} // marathon