#pragma once

// std libs
#include <queue>
#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>
#include <string>

// internal
#include "core/i_system.hpp"
#include "events/events.hpp"

namespace marathon {

namespace events {

class IEventSystem : public ISystem {
public:
    IEventSystem(const std::string& name);
    virtual ~IEventSystem() = default;

    // system interface
    virtual bool Init(BackendFlags flags) = 0;
    virtual void Quit() = 0;

    // event interface
    virtual bool PollEvent(Event& e) = 0;
    virtual bool WaitEvent(Event& e) = 0;
};

} // events
    
} //  marathon
