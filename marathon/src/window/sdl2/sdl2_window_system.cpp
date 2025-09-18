#include "window/sdl2/sdl2_window_system.hpp"

// std lib
#include <iostream>
#include <stdexcept>

// external
#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>

// internal
#include "core/logger.hpp"

namespace marathon {

namespace window {

namespace sdl2 {

SDL2WindowSystem::SDL2WindowSystem()
    : IWindowSystem("marathon.window.sdl2.sdl2_window_system") {}

SDL_Window* SDL2WindowSystem::FindWindow(WindowID win) {
    auto it = _idTable.find(win);
    if (it != _idTable.end()) {
        MT_CORE_WARN("window/sdl2/sdl2_window_system.cpp: Can't find window matching ID.");
        return nullptr;
    }
    return it->second;
}


bool SDL2WindowSystem::Init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        MT_CORE_WARN("window/sdl2/sdl2_window_system.cpp: SDL_Init Error = {}", SDL_GetError());
        return false;
    }
    return true;
}

void SDL2WindowSystem::Quit() {
    for (auto& pair : _idTable) {
        DestroyWindow(pair.first);
    }
    _idTable.clear();
    SDL_Quit();
}

WindowID SDL2WindowSystem::CreateWindow(const std::string& title, int w, int h) {
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        window_flags);

    if (!window) {
        std::cerr << "SDL2WindowSystem: Failed to create window: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to create SDL window");
    }

    WindowID id = _nextID++;
    _idTable[id] = window;

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL2WindowSystem: Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        _idTable.erase(id);
        throw std::runtime_error("Failed to create OpenGL context");
    }

    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "SDL2WindowSystem: Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        _idTable.erase(id);
        throw std::runtime_error("Failed to initialize GLEW");
    }

    return id;
}

void SDL2WindowSystem::DestroyWindow(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_DestroyWindow(w);
}

void SDL2WindowSystem::SwapFrame(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_GL_SwapWindow(w);
}

void* SDL2WindowSystem::GetContext(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return nullptr;
    }
    return SDL_GL_GetCurrentContext();
}

void SDL2WindowSystem::SetWindowMinSize(WindowID win, int minWidth, int minHeight) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetWindowMinimumSize(w, minWidth, minHeight);
}

void SDL2WindowSystem::GetWindowMinSize(WindowID win, int& minWidth, int& minHeight) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_GetWindowMinimumSize(w, &minWidth, &minHeight);
}

void SDL2WindowSystem::SetWindowSize(WindowID win, int width, int height) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetWindowSize(w, width, height);
}

void SDL2WindowSystem::GetWindowSize(WindowID win, int& width, int& height) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_GetWindowSize(w, &width, &height);
}

void SDL2WindowSystem::SetCursorCapture(WindowID win, bool capture) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetRelativeMouseMode((SDL_bool)capture);
}

bool SDL2WindowSystem::GetCursorCapture(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return false;
    }
    return SDL_GetRelativeMouseMode() == SDL_TRUE;
}

bool SDL2WindowSystem::IsVisible(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return false;
    }
    Uint32 flags = SDL_GetWindowFlags(w);
    return (flags & SDL_WINDOW_SHOWN) != 0;
}

void SDL2WindowSystem::Show(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_ShowWindow(w);
}

void SDL2WindowSystem::Hide(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_HideWindow(w);
}

} // sdl2

} // window

} // marathon
