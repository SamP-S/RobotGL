#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <functional>
#include <iostream>

#include "core/module.hpp"

namespace marathon {

namespace window {

struct WindowConfig {
    std::string title = "Untitled Window";
    int minWidth = 800;
    int minHeight = 600;
    int width = 1280;
    int height = 720;
    int resizable = 1;
    int borderless = 0;
    int centred = 1;
    int fullscreen = 0;
};

class Window : public marathon::Module {
protected:
    Window(const std::string& name);
    
public:
    virtual ~Window() = default;

    static Window& Instance();
    
    // module interface
    virtual bool Boot() = 0;
    virtual bool Shutdown() = 0;

    // common
    virtual void SwapFrame() = 0;
    
    // properties
    virtual void SetWindowMinSize(int minWidth, int minHeight) = 0;
    virtual void GetWindowMinSize(int& minWidth, int& minHeight) = 0;
    virtual void SetWindowSize(int width, int height) = 0;
    virtual void GetWindowSize(int& width, int& height) = 0;
    virtual void SetCursorCapture(bool capture) = 0;
    virtual bool GetCursorCapture() = 0;
    virtual bool IsOpen() = 0;
    virtual void Close() = 0;
};

} // window

} // marathon