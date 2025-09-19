#include "events/events.hpp"

// internal
#include "core/logger.hpp"
#include "events/i_event_system.hpp"
#include "events/sdl2/sdl2_event_system.hpp"

namespace marathon {

namespace events {

namespace {
    static IEventSystem* instance = nullptr;
}

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
bool Init(BackendFlags flags) {
    if (instance) {
        MT_CORE_WARN("events/events.cpp: Cannot init an already initialised system.");
        return false;
    }

    /// TODO: replace with backend enum flags select
    instance = new sdl2::SDL2EventSystem();
    if (!instance->Init(flags)) {
        MT_CORE_ERROR("events/events.cpp: Failed to init event system.");
        delete instance;
        instance = nullptr;
        return false;
    }

    return true;
}

void Quit() {
    if (!instance) {
        MT_CORE_WARN("events/events.cpp: Cannot quit an uninitialised system.");
        return;
    }

    instance->Quit();
    delete instance;
    instance = nullptr;
}

bool PollEvent(Event& e) {
    if (!instance) {
        MT_CORE_WARN("events/events.cpp: Event system not initialised.");
        return false;
    }
    return instance->PollEvent(e);
}

bool WaitEvent(Event& e) {
    if (!instance) {
        MT_CORE_WARN("events/events.cpp: Event system not initialised.");
        return false;
    }
    return instance->WaitEvent(e);
}

} // events
    
} // marathon