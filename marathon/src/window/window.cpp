#include "window/window.hpp"

// internal
#include "backend_manager.hpp"
#include "core/logger.hpp"
#include "window/i_window_system.hpp"

namespace marathon {

namespace window {

// window API facade
bool Init() {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return false;;
    }
    return instance->Init();
}

void Quit() {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->Quit();
}

// renderer
WindowID CreateWindow(const std::string& title, int w, int h) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return 0;
    }
    return instance->CreateWindow(title, w, h);
}

void DestroyWindow(WindowID win) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->DestroyWindow(win);
}

void* GetRenderContext(WindowID windowID) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return nullptr;
    }
    return instance->GetRenderContext(windowID);
}

void* GetNativeWindow(WindowID windowID) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return nullptr;
    }
    return instance->GetNativeWindow(windowID);
}

void SwapFrame(WindowID windowID) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->SwapFrame(windowID);
}

// getter/setters
void SetWindowMinSize(WindowID windowID, int minWidth, int minHeight) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->SetWindowMinSize(windowID, minWidth, minHeight);
}

void GetWindowMinSize(WindowID windowID, int& minWidth, int& minHeight) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->GetWindowMinSize(windowID, minWidth, minHeight);
}

void SetWindowSize(WindowID windowID, int width, int height) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->SetWindowSize(windowID, width, height);
}

void GetWindowSize(WindowID windowID, int& width, int& height) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->GetWindowSize(windowID, width, height);
}

void SetCursorCapture(WindowID windowID, bool capture) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->SetCursorCapture(windowID, capture);
}

bool GetCursorCapture(WindowID windowID) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return false;
    }
    return instance->GetCursorCapture(windowID);
}

void Show(WindowID windowID) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->Show(windowID);
}

void Hide(WindowID windowID) {
    auto* instance = BackendManager::Instance().GetSystem<IWindowSystem>(SYS_WINDOW);
    if (!instance) {
        return;
    }
    instance->Hide(windowID);
}

} // window
    
} // marathon
