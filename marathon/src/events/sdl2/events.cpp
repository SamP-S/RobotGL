#include "SDL2/SDL_events.h"
#include "SDL2/SDL.h"

#include "core/logger.hpp"
#include "events/sdl2/events.hpp"

#include <iostream>

namespace marathon {

namespace events {

namespace sdl2 {


Events::Events()
    : marathon::events::Events("marathon.events.sdl2") {}

Events::~Events() {}

bool Events::Boot() {
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
        MT_CORE_ERROR("events/sdl2/events.cpp: SDL_Init Error = {}", SDL_GetError());
    }
    // TODO: actually validate
    _active = true;
    return true;
}
bool Events::Shutdown() {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    // TODO: actually validate
    _active = false;
    return true;
}

void Events::Fetch() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        std::shared_ptr<Signal> s = Convert(e);
        if (s != nullptr) {
            Events::Push(s);
        }
    }
}
void Events::Clear() {
    SDL_Event e;
	while (SDL_PollEvent(&e)) {}
    Events::Clear();
}

/// TODO:
// add parsing for more event types controller/touch/drop/audio
// more rigorously parse events rather than current dummy/generic stuff


std::shared_ptr<Signal> Events::Convert(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            return std::make_shared<Signal>("quit", std::unordered_map<std::string, EventProperty>());
        case SDL_WINDOWEVENT:
            return ConvertWindowEvent(e);
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            return ConvertMouseEvent(e);
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return ConvertKeyboardEvent(e);
        default:
            return nullptr;
    }
}

std::shared_ptr<Signal> Events::ConvertWindowEvent(const SDL_Event& e) {
    std::unordered_map<std::string, EventProperty> data;
    switch (e.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            return std::make_shared<Signal>("window.close", data);
        case SDL_WINDOWEVENT_RESIZED:
            data["width"] = EventProperty(e.window.data1);
            data["height"] = EventProperty(e.window.data2);
            return std::make_shared<Signal>("window.resize", data);
        case SDL_WINDOWEVENT_MOVED:
            data["x"] = EventProperty(e.window.data1);
            data["y"] = EventProperty(e.window.data2);
            return std::make_shared<Signal>("window.move", data);
        case SDL_WINDOWEVENT_MINIMIZED:
            return std::make_shared<Signal>("window.minimize", data);
        case SDL_WINDOWEVENT_MAXIMIZED:
            return std::make_shared<Signal>("window.maximize", data);
        case SDL_WINDOWEVENT_RESTORED:
            return std::make_shared<Signal>("window.restore", data);
        case SDL_WINDOWEVENT_ENTER:
            return std::make_shared<Signal>("window.enter", data);
        case SDL_WINDOWEVENT_LEAVE:
            return std::make_shared<Signal>("window.leave", data);
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            return std::make_shared<Signal>("window.focus.gain", data);
        case SDL_WINDOWEVENT_FOCUS_LOST:
            return std::make_shared<Signal>("window.focus.lost", data);
        default:
            return nullptr;
    }
}
std::shared_ptr<Signal> Events::ConvertMouseEvent(const SDL_Event& e) {
    std::unordered_map<std::string, EventProperty> data;
    switch (e.type) {
        case SDL_MOUSEMOTION:
            data["x"] = EventProperty(e.motion.x);
            data["y"] = EventProperty(e.motion.y);
            data["dx"] = EventProperty(e.motion.xrel);
            data["dy"] = EventProperty(e.motion.yrel);
            return std::make_shared<Signal>("mouse.move", data);
        case SDL_MOUSEBUTTONDOWN:
            data["x"] = EventProperty(e.button.x);
            data["y"] = EventProperty(e.button.y);
            data["button"] = EventProperty(e.button.button);
            return std::make_shared<Signal>("mouse.down", data);
        case SDL_MOUSEBUTTONUP:
            data["x"] = EventProperty(e.button.x);
            data["y"] = EventProperty(e.button.y);
            data["button"] = EventProperty(e.button.button);
            return std::make_shared<Signal>("mouse.up", data);
        case SDL_MOUSEWHEEL:
            data["x"] = EventProperty(e.wheel.x);
            data["y"] = EventProperty(e.wheel.y);
            return std::make_shared<Signal>("mouse.wheel", data);
        default:
            return nullptr;
    }
}
std::shared_ptr<Signal> Events::ConvertKeyboardEvent(const SDL_Event& e) {
    std::unordered_map<std::string, EventProperty> data;
    switch (e.type) {
        case SDL_KEYDOWN:
            data["key"] = EventProperty(e.key.keysym.sym);
            data["mod"] = EventProperty(e.key.keysym.mod);
            return std::make_shared<Signal>("key.down", data);
        case SDL_KEYUP:
            data["key"] = EventProperty(e.key.keysym.sym);
            data["mod"] = EventProperty(e.key.keysym.mod);
            return std::make_shared<Signal>("key.up", data);
        default:
            return nullptr;
    }
}


} // sdl2

} // events

} // marathon