#include "graphics/i_graphics_system.hpp"

namespace marathon {

namespace graphics {

IGraphicsSystem::IGraphicsSystem(const std::string& name)
    : ISystem(SystemID::SYS_GRAPHICS, name) {}

} // graphics

} // marathon
