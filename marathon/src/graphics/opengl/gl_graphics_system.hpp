#pragma once

// internal
#include "graphics/i_graphics_system.hpp"

namespace marathon {

namespace graphics {

class GLGraphicsSystem : public IGraphicsSystem {
public:
    GLGraphicsSystem();
    ~GLGraphicsSystem() = default;

    // system interface
    bool Init(BackendFlags flags) override;
    void Quit() override;

    // grpahics interface
    void Clear() override;
    void SetClearColour(float r, float g, float b, float a) override;

};

} // graphics

} // marathon
