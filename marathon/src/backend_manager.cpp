#include "backend_manager.hpp"

// internal
#include "core/logger.hpp"

namespace marathon {

BackendManager& BackendManager::Instance() {
    static BackendManager instance;
    return instance;
}

bool BackendManager::Impl(BackendFlags flags) {
    MT_CORE_INFO("backend_manager.cpp: backend impl manager impl.");
}

bool BackendManager::Init() {
    if (!_valid_impl) {
        MT_CORE_CRITICAL("backend_manager.cpp: bm can't init, no valid impl call.");
    }
    MT_CORE_INFO("backend_manager.cpp: backend impl manager init.");
}

void BackendManager::Quit() {
    MT_CORE_INFO("backend_manager.cpp: backend impl manager quitting.");
}

template<typename T>
T* BackendManager::GetSystem(SystemID sys) {
    if (!_systems[sys]) {
        MT_CORE_ERROR("backend_manager.cpp: no backend imp for system ({})", sys);
        return nullptr;
    }
    return static_cast<T*>(_systems[sys]);
}

}
