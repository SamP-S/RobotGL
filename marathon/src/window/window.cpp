#include "window/window.hpp"

// internal
#include "core/logger.hpp"
#include "window/i_window_system.hpp"
#include "window/sdl2/sdl2_window_system.hpp"

namespace marathon {

namespace window {

namespace {
    static IWindowSystem* instance = nullptr;
}

// events API facade
bool Init() {
    if (instance) {
        MT_CORE_WARN("window/window.cpp: Cannot init an already initialised system.");
        return false;
    }

    /// TODO: replace with backend enum flags select
    instance = new sdl2::SDL2WindowSystem();
    if (!instance->Init(flags)) {
        MT_CORE_ERROR("window/window.cpp: Failed to init event system.");
        delete instance;
        instance = nullptr;
        return false;
    }

    return true;
}

void Quit() {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Cannot quit an uninitialised system.");
        return;
    }

    instance->Quit();
    delete instance;
    instance = nullptr;
}

// renderer
WindowID CreateWindow(const std::string& title, int w, int h) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return 0;
    }
    return instance->CreateWindow(title, w, h);
}

void DestroyWindow(WindowID win) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->DestroyWindow(win);
}

void* GetRenderContext(WindowID windowID) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return nullptr;
    }
    return instance->GetRenderContext(windowID);
}
void* GetNativeWindow(WindowID windowID) {
   if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return nullptr;
    }
    return instance->GetNativeWindow(windowID);
}
void SwapFrame(WindowID windowID) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->SwapFrame(windowID);
}

// getter/setters
void SetWindowMinSize(WindowID windowID, int minWidth, int minHeight) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->SetWindowMinSize(windowID, minWidth, minHeight);
}
void GetWindowMinSize(WindowID windowID, int& minWidth, int& minHeight) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->GetWindowMinSize(windowID, minWidth, minHeight);
}
void SetWindowSize(WindowID windowID, int width, int height) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->SetWindowSize(windowID, width, height);
}
void GetWindowSize(WindowID windowID, int& width, int& height) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->GetWindowSize(windowID, width, height);
}
void SetCursorCapture(WindowID windowID, bool capture) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->SetCursorCapture(windowID, capture);
}
bool GetCursorCapture(WindowID windowID) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return false;
    }
    return instance->GetCursorCapture(windowID);
}
void Show(WindowID windowID) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->Show(windowID);
}
void Hide(WindowID windowID) {
    if (!instance) {
        MT_CORE_WARN("window/window.cpp: Window system not instanced.");
        return;
    }
    instance->Hide(windowID);
}

} // window
    
} // marathon
