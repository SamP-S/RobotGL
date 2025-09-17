// internal
#include "events/event.hpp"

namespace marathon {

namespace events {

Event::Event(const std::string& name, const std::unordered_map<std::string, EventProperty>& data)
    : name(name), data(data) {}

} // namespace events

} // namespace marathon