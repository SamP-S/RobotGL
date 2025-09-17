#pragma once

// std lib
#include <string>

// internal
#include "core/i_system.hpp"

namespace marathon {

namespace time {

class ITimeSystem : public core::ISystem {
public:
    ITimeSystem(const std::string& name);
    virtual ~ITimeSystem();

    // system interface
    virtual bool Init() = 0;
    virtual void Quit() = 0;

    // time interface
    virtual double Tick() = 0;
    virtual double Time() = 0;
};

} // time

} // marathon