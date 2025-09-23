#include "marathon.hpp"

// std lib
#include <algorithm>
#include <array>

// external
#include "SDL2/SDL.h"

// internal
#include "backend_manager.hpp"
#include "core/logger.hpp"

namespace marathon {

bool Impl(BackendFlags flags) {
    return BackendManager::Instance().Impl(flags);
}

bool Init() {
    return BackendManager::Instance().Init();
}

void Quit() {
    BackendManager::Instance().Quit();
}

} // marathon