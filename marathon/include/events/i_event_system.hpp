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

namespace marathon {

namespace events {

class IEventSystem : public ISystem {
public:
    IEventSystem();
    ~IEventSystem();

    bool Init(std::unique_ptr<IEventSystem> impl);  // differing from ISystem
    bool Quit();
    bool PollEvent(Event& e);
    bool WaitEvent(Event& e);
};

} // events
    
} //  marathon
