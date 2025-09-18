#pragma once

namespace marathon {

namespace window {

// window API facade
bool Init();
void Quit();

enum WindowFlags : int32_t {
    WINDOW_NO_FLAGS             = 0x01 << 0,
    WINDOW_FULLSCREEN           = 0x01 << 1,
    WINDOW_OCCLUDED             = 0x01 << 2,
    WINDOW_HIDDEN               = 0x01 << 3,
    WINDOW_RESIZABLE            = 0x01 << 4,
    WINDOW_MINIMIZED            = 0x01 << 5,
    WINDOW_MAXIMIZED            = 0x01 << 6,
};

class Window {
private:

public:
    // renderer
    void* GetGLContext();
    void SwapFrame();

    // getter/setters
    void SetWindowMinSize(int minWidth, int minHeight);
    void GetWindowMinSize(int& minWidth, int& minHeight);
    void SetWindowSize(int width, int height);
    void GetWindowSize(int& width, int& height);
    void SetCursorCapture(bool capture);
    bool GetCursorCapture();
    void Show();
    void Hide();
};

Window* CreateWindow(const std::string& title, int w, int h, WindowFlags flags);
void DestroyWindow();

} // window
    
} //  marathon
