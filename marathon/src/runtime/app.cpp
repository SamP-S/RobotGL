#include "runtime/app.hpp"

#include "marathon.hpp"
#include "window/window.hpp"
#include "events/events.hpp"
#include "time/time.hpp"
#include "core/logger.hpp"

namespace marathon {

App::App() {}
App::~App() {}

/// TODO: ensure Init/Quit are successful

// boot systems and modules
void App::Boot() {
    if (marathon::Init() != 0) {
        MT_ENGINE_CRITICAL("App::Boot(): Failed to initialise marathon. :(");
        _mFailed = true;
    }
}

// shutdown systems and modules
void App::Shutdown() {
    if (marathon::Quit() != 0) {
        MT_ENGINE_CRITICAL("App::Shutdown(): Failed to initialise marathon. :(");
        _mFailed = true;
    }
}

void App::Run() {
    // internal boot all systems and modules
    Boot();
    if (_mFailed) {
        return;
    }

    // user defined start
    Start();

    // main game loop
    while (!_mQuit) {
        // fetch events from backend ready for polling
        events::Events::Instance().Fetch();
        
        // interactive tick
        double dt = time::Time::Instance().Tick();
        // user defined update
        Update(dt);
        
        // swap frame shown
        window::Window::Instance().SwapFrame();
    }

    // internal shutdown safely
    Shutdown();
    if (_mFailed) {
        return;
    }
}

void App::Quit() {
    _mQuit = true;
}


} // namespace marathon