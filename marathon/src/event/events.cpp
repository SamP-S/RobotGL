#include "events/events.hpp"
#include "events/sdl2/events.hpp"

namespace marathon {

namespace events {

Signal::Signal(const std::string& name, const std::unordered_map<std::string, EventProperty>& data)
    : name(name), data(data) {}

// Constructor
Events::Events(const std::string& name)
    : marathon::Module(marathon::ModuleType::EVENTS, name) {}

// Singleton instance accessor
Events& Events::Instance() {
    static Events* instance;
    if (!instance)
        instance = new marathon::events::sdl2::Events();
    return *instance;
}

// Common iterative access
bool Events::Poll(std::shared_ptr<Signal>& event) {
    if (_queue.empty())
        return false;
    event = _queue.front();
    _queue.pop();
    return true;
}
    
void Events::Push(std::shared_ptr<Signal> event) {
    _queue.push(event);
}

void Events::Clear() {
    while (!_queue.empty()) {
        _queue.pop();
    }
}

} // events
    
} // marathon