#pragma once

// must define before SDL_opengl.h otherwise GLEW breaks
#define GL_VERSION_4_4
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <functional>
#include <iostream>

#include "core/module.hpp"
#include "window/window.hpp"

namespace marathon {

namespace window {

namespace sdl2 {


//// TODO:
// Abstract renderer to allow for Vulkan, use cfg structs
// Abstract so not SDL specific implementation
// Implement all the window  property get/set
// Rm event handling from here should be put in event class
// Add support for OpenGL 4.6
// Convert opengl config to use bools

//// NOTE:
// can add sRGB to OpenGLConfig
// can add SDL_GL_CONTEXT_DEBUG_FLAG for better OpenGL debugging

class Window : public marathon::window::Window {
private:
    WindowConfig _windowConfig;
    SDL_GLContext _openglContext;
    SDL_Window* _window;
    bool _isOpen;
   
public:
    Window();
    ~Window() override;

    // module interface
    bool Boot() override;
    bool Shutdown() override;

    // common
    void* GetGLContext();
    SDL_Window* GetWindow();

    void SwapFrame() override;

    // properties
    void SetWindowMinSize(int minWidth, int minHeight) override;
    void GetWindowMinSize(int& minWidth, int& minHeight) override;
    void SetWindowSize(int width, int height) override;
    void GetWindowSize(int& width, int& height) override;
    void SetCursorCapture(bool capture) override;
    bool GetCursorCapture() override;
    bool IsOpen() override;
    void Close() override;
};

} // sdl2

} // window

} // marathon