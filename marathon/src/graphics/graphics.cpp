#include "graphics/graphics.hpp"

// internal
#include "core/logger.hpp"
#include "graphics/i_graphics_system.hpp"
#include "graphics/opengl/gl_graphics_system.hpp"

namespace marathon {

namespace graphics {

namespace {
    static IGraphicsSystem* instance = nullptr;
}

// system interface
bool Init(BackendFlags flags) {
    if (instance) {
        MT_CORE_WARN("graphics/graphics.cpp: Cannot init an already init system.");
        return false;
    }

    instance = new gl::GLGraphicsSystem();
    if (!instance->Init(flags)) {
        MT_CORE_ERROR("graphics/graphics.cpp: Failed to init graphics system.");
        delete instance;
        instance = nullptr;
        return false;
    }

    return true;
}

void Quit() {
    if (!instance) {
        MT_CORE_WARN("graphics/graphics.cpp: Cannot quit an uninitialised system.");
        return;
    }

    instance->Quit();
    delete instance;
    instance = nullptr;
}

// graphics API facade
void Clear() {
    if (!instance) {
        MT_CORE_WARN("graphics/graphics.cpp: Cannot quit an uninitialised system.");
        return;
    }
    instance->Clear();
}
void SetClearColour(float r, float g, float b, float a) {
    if (!instance) {
        MT_CORE_WARN("graphics/graphics.cpp: Cannot quit an uninitialised system.");
        return;
    }
    instance->SetClearColour(r, g, b, a);
}

} // graphics

} // marathon
