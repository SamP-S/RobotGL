#include "window/window.hpp"
#include "window/sdl2/window.hpp"

namespace marathon {

namespace window {

Window::Window(const std::string& name)
: Module(ModuleType::WINDOW, name) {}

// renderer is singleton
Window& Window::Instance() {
    static Window* instance;
    if (!instance)
        instance = new marathon::window::sdl2::Window();
    return *instance;
}

} // window

} // marathon
