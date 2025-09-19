#pragma once

// std lib
#include <variant>

// internal
#include "core/i_system.hpp"

namespace marathon {

namespace events {

typedef std::variant<int, float, double, bool, std::string> EventProperty;

/// @brief Represents an event with a name and associated data.
/// @details An event consists of constant data, so it cannot be modified.
/// Events should be handled as is and discarded after use.
struct Event {
    std::string name;
    std::unordered_map<std::string, EventProperty> data;

    Event() = default;
    Event(const std::string& name, const std::unordered_map<std::string, EventProperty>& data);
};

// events API facade
bool Init(BackendFlags flags);
void Quit();
bool PollEvent(Event& e);
bool WaitEvent(Event& e);

/// TODO: Impl for pushing events
/// TODO: Impl for wait until timeout

} // events
    
} //  marathon
