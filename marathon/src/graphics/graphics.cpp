#include "graphics/graphics.hpp"

// internal
#include "backend_manager.hpp"
#include "core/logger.hpp"
#include "graphics/i_graphics_system.hpp"

namespace marathon {

namespace graphics {

// system interface
bool Init() {
    auto* instance = BackendManager::Instance().GetSystem<IGraphicsSystem>(SYS_GRAPHICS);
    if (!instance) {
        return false;
    }
    return instance->Init();
}

void Quit() {
    auto* instance = BackendManager::Instance().GetSystem<IGraphicsSystem>(SYS_GRAPHICS);
    if (!instance) {
        return;
    }
    instance->Quit();
}

// graphics API facade
void Clear() {
    auto* instance = BackendManager::Instance().GetSystem<IGraphicsSystem>(SYS_GRAPHICS);
    if (!instance) {
        return;
    }
    instance->Clear();
}

void SetClearColour(float r, float g, float b, float a) {
    auto* instance = BackendManager::Instance().GetSystem<IGraphicsSystem>(SYS_GRAPHICS);
    if (!instance) {
        return;
    }
    instance->SetClearColour(r, g, b, a);
}

} // graphics

} // marathon
