#include "window/i_window_system.hpp"

namespace marathon {

namespace window {

IWindowSystem::IWindowSystem(const std::string& name)
    : ISystem(SYS_WINDOW, {}, name) {}

} // namespace window

} // namespace marathon