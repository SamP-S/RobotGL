#pragma once

// std lib
#include <string>

// internal
#include "core/i_system.hpp"
#include "window/window.hpp"

namespace marathon {

namespace window {

class IWindowSystem : public ISystem {
public:
    IWindowSystem(const std::string& name);
    virtual ~IWindowSystem() = default;
    
    // system interface
    virtual bool Init() = 0;
    virtual void Quit() = 0;

    // window interface
    virtual WindowID CreateWindow(const std::string& title, int w, int h) = 0;
    virtual void DestroyWindow(WindowID win) = 0;
    virtual void* GetRenderContext(WindowID win) = 0;
    virtual void* GetNativeWindow(WindowID win) = 0;
    virtual void SwapFrame(WindowID win) = 0;
    virtual void SetWindowMinSize(WindowID win, int minWidth, int minHeight) = 0;
    virtual void GetWindowMinSize(WindowID win, int& minWidth, int& minHeight) = 0;
    virtual void SetWindowSize(WindowID win, int width, int height) = 0;
    virtual void GetWindowSize(WindowID win, int& width, int& height) = 0;
    virtual void SetCursorCapture(WindowID win, bool capture) = 0;
    virtual bool GetCursorCapture(WindowID win) = 0;
    virtual bool IsVisible(WindowID win) = 0;
    virtual void Show(WindowID win) = 0;
    virtual void Hide(WindowID win) = 0;
};

} // window

} // marathon