// std lib
#include <iostream>

// external
#include "SDL2/SDL_events.h"
#include "SDL2/SDL.h"

// internal
#include "core/logger.hpp"
#include "events/sdl2/sdl2_event_system.hpp"

namespace marathon {

namespace events {

namespace sdl2 {

SDL2EventSystem::SDL2EventSystem()
    : IEventSystem("marathon.events.sdl2.sdl2_event_system") {}

SDL2EventSystem::~SDL2EventSystem() {}

bool SDL2EventSystem::Init() {
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        MT_CORE_ERROR("events/sdl2/sdl2_event_system.cpp: SDL_Init Error = {}", SDL_GetError());
        return false;
    }
    return true;
}
void SDL2EventSystem::Quit() {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    // TODO: actually validate
}

/** @brief get next event in queue
 *  @return 1 if more events in queue, 0 otherwise
 */
bool SDL2EventSystem::PollEvent(Event& e) {
    SDL_Event sdl_e;
    int pending = SDL_PollEvent(&sdl_e);
    Convert(sdl_e, e);
    return pending;
}

/** @brief wait for next event
 *  @return 1 if success, 0 if errror
 */
bool SDL2EventSystem::WaitEvent(Event& e) {
    SDL_Event sdl_e;
	int pending = SDL_WaitEvent(&sdl_e);
    Convert(sdl_e, e);
    return pending;
}

/// TODO:
// add parsing for more event types controller/touch/drop/audio
// more rigorously parse events rather than current dummy/generic stuff
bool SDL2EventSystem::Convert(const SDL_Event& sdl_e, Event& e) {
    switch (sdl_e.type) {
        case SDL_QUIT:
            e = Event("quit", std::unordered_map<std::string, EventProperty>());
            return true;
        case SDL_WINDOWEVENT:
            return ConvertWindowEvent(sdl_e, e);
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            return ConvertMouseEvent(sdl_e, e);
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return ConvertKeyboardEvent(sdl_e, e);
        default:
            e = Event("error", std::unordered_map<std::string, EventProperty>());
            return false;
    }
}

bool SDL2EventSystem::ConvertWindowEvent(const SDL_Event& sdl_e, Event& e) {
    std::unordered_map<std::string, EventProperty> data;
    switch (sdl_e.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            e = Event("window.close", data);
            return true;
        case SDL_WINDOWEVENT_RESIZED:
            data["width"] = EventProperty(sdl_e.window.data1);
            data["height"] = EventProperty(sdl_e.window.data2);
            e = Event("window.resize", data);
            return true;
        case SDL_WINDOWEVENT_MOVED:
            data["x"] = EventProperty(sdl_e.window.data1);
            data["y"] = EventProperty(sdl_e.window.data2);
            e = Event("window.move", data);
            return true;
        case SDL_WINDOWEVENT_MINIMIZED:
            e = Event("window.minimize", data);
            return true;
        case SDL_WINDOWEVENT_MAXIMIZED:
            e = Event("window.maximize", data);
            return true;
        case SDL_WINDOWEVENT_RESTORED:
            e = Event("window.restore", data);
            return true;
        case SDL_WINDOWEVENT_ENTER:
            e = Event("window.enter", data);
            return true;
        case SDL_WINDOWEVENT_LEAVE:
            e = Event("window.leave", data);
            return true;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            e = Event("window.focus.gain", data);
            return true;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            e = Event("window.focus.lost", data);
            return true;
        default:
            e = Event("error", std::unordered_map<std::string, EventProperty>());
            return false;
    }
}

bool SDL2EventSystem::ConvertMouseEvent(const SDL_Event& sdl_e, Event& e) {
    std::unordered_map<std::string, EventProperty> data;
    switch (sdl_e.type) {
        case SDL_MOUSEMOTION:
            data["x"] = EventProperty(sdl_e.motion.x);
            data["y"] = EventProperty(sdl_e.motion.y);
            data["dx"] = EventProperty(sdl_e.motion.xrel);
            data["dy"] = EventProperty(sdl_e.motion.yrel);
            e = Event("mouse.move", data);
            return true;
        case SDL_MOUSEBUTTONDOWN:
            data["x"] = EventProperty(sdl_e.button.x);
            data["y"] = EventProperty(sdl_e.button.y);
            data["button"] = EventProperty(sdl_e.button.button);
            e = Event("mouse.down", data);
            return true;
        case SDL_MOUSEBUTTONUP:
            data["x"] = EventProperty(sdl_e.button.x);
            data["y"] = EventProperty(sdl_e.button.y);
            data["button"] = EventProperty(sdl_e.button.button);
            e = Event("mouse.up", data);
            return true;
        case SDL_MOUSEWHEEL:
            data["x"] = EventProperty(sdl_e.wheel.x);
            data["y"] = EventProperty(sdl_e.wheel.y);
            e = Event("mouse.wheel", data);
            return true;
        default:
            e = Event("error", std::unordered_map<std::string, EventProperty>());
            return false;
    }
}

bool SDL2EventSystem::ConvertKeyboardEvent(const SDL_Event& sdl_e, Event& e) {
    std::unordered_map<std::string, EventProperty> data;
    switch (sdl_e.type) {
        case SDL_KEYDOWN:
            data["key"] = EventProperty(sdl_e.key.keysym.sym);
            data["mod"] = EventProperty(sdl_e.key.keysym.mod);
            e = Event("key.down", data);
            return true;
        case SDL_KEYUP:
            data["key"] = EventProperty(sdl_e.key.keysym.sym);
            data["mod"] = EventProperty(sdl_e.key.keysym.mod);
            e = Event("key.up", data);
            return true;
        default:
            e = Event("error", std::unordered_map<std::string, EventProperty>());
            return false;
    }
}


} // sdl2

} // events

} // marathon