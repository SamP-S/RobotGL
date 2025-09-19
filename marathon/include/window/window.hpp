#pragma once

// internal
#include "core/i_system.hpp"

namespace marathon {

namespace window {

typedef uint32_t WindowID;

enum WindowFlags : int32_t {
    WINDOW_NO_FLAGS             = 0x00,
    WINDOW_FULLSCREEN           = 0x01,
    WINDOW_OCCLUDED             = 0x02,
    WINDOW_HIDDEN               = 0x04,
    WINDOW_RESIZABLE            = 0x08,
    WINDOW_MINIMIZED            = 0x10,
    WINDOW_MAXIMIZED            = 0x20,
};

// system interface
bool Init(BackendFlags flags);
void Quit();

// window API facade
WindowID CreateWindow(const std::string& title, int w, int h, WindowFlags flags);
void DestroyWindow(WindowID win);
void* GetRenderContext(WindowID win);
void* GetNativeWindow(WindowID win);
void SwapFrame(WindowID win);
void SetWindowMinSize(WindowID win, int minWidth, int minHeight);
void GetWindowMinSize(WindowID win, int& minWidth, int& minHeight);
void SetWindowSize(WindowID win, int width, int height);
void GetWindowSize(WindowID win, int& width, int& height);
void SetCursorCapture(WindowID win, bool capture);
bool GetCursorCapture(WindowID win);
void Show(WindowID win);
void Hide(WindowID win);

} // window
    
} //  marathon
