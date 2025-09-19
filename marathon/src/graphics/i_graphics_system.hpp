#pragma once

// std lib
#include <string>

// internal
#include "core/i_system.hpp"
#include "graphics/graphics.hpp"

namespace marathon {

namespace graphics {

class IGraphicsSystem : public ISystem {
public:
    IGraphicsSystem(const std::string& name);
    virtual ~IGraphicsSystem() = default;

    // system interface
    virtual bool Init(BackendFlags flags) = 0;
    virtual void Quit() = 0;

    // graphics interface
    virtual void Clear() = 0;
    virtual void SetClearColour(float r, float g, float b, float a) = 0;

};

} // graphics

} // marathon
