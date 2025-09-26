#pragma once

// std lib
#include <unordered_map>

// external
#include <GL/glew.h>
#include <SDL2/SDL.h>

// internal
#include "window/i_window_system.hpp"

namespace marathon {

namespace window {

namespace imgui_sdl2 {

class ImGuiSDL2WindowSystem : public IWindowSystem {
private:
    int _nextID = 1;
    std::unordered_map<WindowID, SDL_Window*> _idTable = {};
    
    SDL_Window* FindWindow(WindowID win);

public:
    ImGuiSDL2WindowSystem();
    ~ImGuiSDL2WindowSystem() = default;
    
    // system interface
    bool Init() override;
    void Quit() override;

    // window interface
    void BeginFrame(WindowID win) override;
    void EndFrame(WindowID win) override;

    // window factory
    WindowID CreateWindow(const std::string& title, int w, int h) override;
    void DestroyWindow(WindowID win) override;

    // native handles
    void* GetRenderContext(WindowID win) override;
    void* GetNativeWindow(WindowID win) override;
    
    // window getters/setters
    std::string GetWindowTitle(WindowID win) override;
    void SetWindowTitle(WindowID win, const std::string& title) override;
    void SetWindowMinSize(WindowID win, int minWidth, int minHeight) override;
    void GetWindowMinSize(WindowID win, int& minWidth, int& minHeight) override;
    void SetWindowSize(WindowID win, int width, int height) override;
    void GetWindowSize(WindowID win, int& width, int& height) override;
    void SetCursorCapture(WindowID win, bool capture) override;
    bool GetCursorCapture(WindowID win) override;
    bool IsVisible(WindowID win) override;
    void Show(WindowID win) override;
    void Hide(WindowID win) override;
};

} // imgui sdl2

} // window

} // marathon