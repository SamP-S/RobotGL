#include "events/events.hpp"

// internal
#include "core/logger.hpp"
#include "events/i_event_system.hpp"
#include "events/sdl2/sdl2_event_system.hpp"
#include "backend_manager.hpp"

namespace marathon {

namespace events {

std::ostream& operator<<(std::ostream& os, const EventProperty& ep) {
    std::visit([&os](const auto& value) {
        if constexpr (std::is_same_v<decltype(value), bool>) {
            os << (value ? "true" : "false");
        } else {
            os << value;
        }
    }, ep);
    return os;
}

Event::Event(const std::string& name, const std::unordered_map<std::string, EventProperty>& data)
    : name(name), data(data) {}

std::ostream& operator<<(std::ostream& os, const Event& event) {
    os << "Event Name: " << event.name << std::endl;
    for (const auto& [key, value] : event.data) {
        os << "\tKey: " << key << ", Value: " << value << std::endl;
    }
    return os;
}

// events API facade
bool Init() {
    auto* instance = BackendManager::Instance().GetSystem<IEventSystem>(SYS_EVENTS);
    if (!instance) {
        return false;
    }
    return instance->Init();
}

void Quit() {
    auto* instance = BackendManager::Instance().GetSystem<IEventSystem>(SYS_EVENTS);
    if (!instance) {
        return;
    }

    instance->Quit();
}

bool PollEvent(Event& e) {
    auto* instance = BackendManager::Instance().GetSystem<IEventSystem>(SYS_EVENTS);
    if (!instance) {
        return false;
    }
    return instance->PollEvent(e);
}

bool WaitEvent(Event& e) {
    auto* instance = BackendManager::Instance().GetSystem<IEventSystem>(SYS_EVENTS);
    if (!instance) {
        return false;
    }
    return instance->WaitEvent(e);
}

} // events
    
} // marathon