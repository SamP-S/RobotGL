#include "graphics/opengl/gl_graphics_system.hpp"

// external
#include <GL/glew.h>

namespace marathon {

namespace graphics {

GLGraphicsSystem::GLGraphicsSystem()
    : IGraphicsSystem("marathon.graphics.gl_graphics_system") {}

// system interface
bool GLGraphicsSystem::Init(BackendFlags flags) {
    // create opengl resources
    return true;
}

void GLGraphicsSystem::Quit() {
    // clean opengl resources
}

// graphics interface
void GLGraphicsSystem::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLGraphicsSystem::SetClearColour(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

} // graphics

} // marathon
