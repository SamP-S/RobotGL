#include "backend_manager.hpp"

// internal
#include "core/logger.hpp"

namespace marathon {

void BackendManager::Init(BackendFlags flags) {
    MT_CORE_INFO("backend_manager.cpp: backend impl manager init.");
}

void BackendManager::Quit() {
    MT_CORE_INFO("backend_manager.cpp: backend impl manager quitting.");
}

template<typename T>
T* BackendManager::GetSystem(SystemID sys) {
    return static_cast<T*>(_systems.at(sys));
}

}
