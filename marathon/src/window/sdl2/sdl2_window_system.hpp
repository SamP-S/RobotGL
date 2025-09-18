#pragma once

// std lib
#include <unordered_map>

// external
#include <SDL2/SDL.h>

// internal
#include "window/i_window_system.hpp"

namespace marathon {

namespace window {

namespace sdl2 {

class SDL2WindowSystem : public IWindowSystem {
private:
    int _nextID = 0;
    std::unordered_map<WindowID, SDL_Window*> _idTable = {};
    
    SDL_Window* FindWindow(WindowID win);

public:
    SDL2WindowSystem();
    ~SDL2WindowSystem() = default;
    
    // system interface
    bool Init() override;
    void Quit() override;

    // window interface
    WindowID CreateWindow(const std::string& title, int w, int h) override;
    void DestroyWindow(WindowID win) override;
    void SwapFrame(WindowID win) override;
    void* GetContext(WindowID win) override;
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

} // sdl2

} // window

} // marathon