#pragma once

// std lib
#include <string>
#include <variant>
#include <unordered_map>

namespace marathon {

namespace events {

typedef std::variant<int, float, double, bool, std::string> EventProperty;

// Note: A event consists of const data so it cannot be modified.
// Events should be handled as is and discarded after use.
class Event {
public:
    Event(const std::string& name, const std::unordered_map<std::string, EventProperty>& data);
    ~Event() = default;

    std::string name;
    std::unordered_map<std::string, EventProperty> data;
};

} // events
    
} //  marathon
