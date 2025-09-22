#pragma once

// std lib
#define GL_VERSION_4_4
#include <GL/glew.h>

// internal
#include "graphics/i_graphics_system.hpp"

namespace marathon {

namespace graphics {

namespace gl {

class GLGraphicsSystem : public IGraphicsSystem {
private:
    GLuint _fbo = 0;

public:
    GLGraphicsSystem();
    ~GLGraphicsSystem() = default;

    // system interface
    bool Init() override;
    void Quit() override;

    // grpahics interface
    void Clear() override;
    void SetClearColour(float r, float g, float b, float a) override;

};

} // gl

} // graphics

} // marathon
