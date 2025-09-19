#include "marathon.hpp"
#include "core/logger.hpp"
#include "time/time.hpp"
#include "window/window.hpp"
#include "events/events.hpp"
#include "graphics/graphics.hpp"

using namespace marathon;

int main() {
    // init
    BackendFlags flags = Backends::SDL2 | Backends::OPENGL;
    if (Init(flags) != 0) {
        MT_ENGINE_CRITICAL("App::Boot(): Failed to initialise marathon. :(");
        return 1;
    }
    window::WindowID w = window::CreateWindow("example", 400, 300);

    // update
    bool isQuit = false;
    while (!isQuit) {
        // fetch events from backend ready for polling
        events::Event event;
        while (events::PollEvent(event)) {
            std::cout << event << std::endl;
            if (event.name == "quit") {
                isQuit = true;
            }
        }
        
        // interactive tick
        double dt = time::Tick();
        double t = time::Time();
        
        // change bg colour over time
        graphics::SetClearColour(
            0.5f + 0.5f * sin(t),
            0.5f + 0.5f * sin(t + 1),
            0.5f + 0.5f * sin(t + 2),
            1.0f
        );
        graphics::Clear();
        
        // swap frame shown
        window::SwapFrame(w);
    }

    // deinit
    if (Quit() != 0) {
        MT_ENGINE_CRITICAL("App::Shutdown(): Failed to initialise marathon. :(");
        return 1;
    }
};
