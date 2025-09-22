#include "time/time.hpp"

// internal
#include "core/logger.hpp"
#include "time/chrono/time_system.hpp"

namespace marathon {

namespace time {

namespace {
    static ITimeSystem* instance = nullptr;
}

// system interface
bool Init() {
    if (instance) {
        MT_CORE_WARN("time/time.cpp: Cannot init an already initialised system.");
        return false;
    }

    instance = new TimeSystem();
    if (!instance->Init(flags)) {
        MT_CORE_ERROR("time/time.cpp: Failed to init time system.");
        delete instance;
        instance = nullptr;
        return false;
    }

    return true;
}
void Quit() {
    if (!instance) {
        MT_CORE_WARN("time/time.cpp: Cannot quit an uninitialised system.");
        return;
    }

    instance->Quit();
    delete instance;
    instance = nullptr;
}

// time interface
double Tick() {
    if (!instance) {
        MT_CORE_WARN("time/time.cpp: Time system not initialised.");
        return 0.0;
    }
    return instance->Tick();
}
double Time() {
    if (!instance) {
        MT_CORE_WARN("time/time.cpp: Time system not initialised.");
        return 0.0;
    }
    return instance->Time();
}

} // time

} // marathon