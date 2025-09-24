#include "time/time.hpp"

// internal
#include "core/logger.hpp"
#include "backend_manager.hpp"

namespace marathon {

namespace time {

// system interface
bool Init() {
    return BackendManager::Instance().GetSystem<ITimeSystem>(SYS_TIME)->Init();
}
void Quit() {
    BackendManager::Instance().GetSystem<ITimeSystem>(SYS_TIME)->Quit();
}

// time interface
double Tick() {
    return BackendManager::Instance().GetSystem<ITimeSystem>(SYS_TIME)->Tick();
}
double Time() {
    return BackendManager::Instance().GetSystem<ITimeSystem>(SYS_TIME)->Time();
}

} // time

} // marathon