#include "marathon.hpp"
#include "core/logger.hpp"
#include "time/time.hpp"
#include "window/window.hpp"
#include "events/events.hpp"

namespace mtn = marathon;

int main() {
    // init
    if (mtn::Init() != 0) {
        MT_ENGINE_CRITICAL("App::Boot(): Failed to initialise marathon. :(");
        return 1;
    }

    // update
    bool isQuit = false;
    while (!isQuit) {
        // fetch events from backend ready for polling
        mtn::events::Events::Instance().Fetch();
        
        // interactive tick
        double dt = mtn::time::Time::Instance().Tick();
        // user defined update
        // FILL ME
        
        // swap frame shown
        mtn::window::Window::Instance().SwapFrame();
    }

    // deinit
    if (mtn::Quit() != 0) {
        MT_ENGINE_CRITICAL("App::Shutdown(): Failed to initialise marathon. :(");
        return 1;
    }
};
