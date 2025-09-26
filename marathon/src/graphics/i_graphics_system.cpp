#include "graphics/i_graphics_system.hpp"

namespace marathon {

namespace graphics {

IGraphicsSystem::IGraphicsSystem(const std::string& name)
    : ISystem(SYS_GRAPHICS, {SYS_WINDOW}, name) {}

} // graphics

} // marathon
