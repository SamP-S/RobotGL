#include "marathon.hpp"
#include "core/logger.hpp"
#include "time/time.hpp"
#include "window/window.hpp"
#include "events/events.hpp"

using namespace marathon;

int main() {
    // init
    if (Init(0) != 0) {
        MT_ENGINE_CRITICAL("App::Boot(): Failed to initialise marathon. :(");
        return 1;
    }
    window::WindowID w = window::CreateWindow("example", 400, 300, window::WindowFlags::WINDOW_NO_FLAGS);

    // update
    bool isQuit = false;
    while (!isQuit) {
        // fetch events from backend ready for polling
        events::Event event;
        while (events::PollEvent(event)) {
            for (const auto& [key, value] : event.data) {
                if (std::holds_alternative<int>(value)) {
                    std::cout << "Key: " << key << ", Value: " << std::get<int>(value) << std::endl;
                } else if (std::holds_alternative<float>(value)) {
                    std::cout << "Key: " << key << ", Value: " << std::get<float>(value) << std::endl;
                } else if (std::holds_alternative<double>(value)) {
                    std::cout << "Key: " << key << ", Value: " << std::get<double>(value) << std::endl;
                } else if (std::holds_alternative<bool>(value)) {
                    std::cout << "Key: " << key << ", Value: " << (std::get<bool>(value) ? "true" : "false") << std::endl;
                } else if (std::holds_alternative<std::string>(value)) {
                    std::cout << "Key: " << key << ", Value: " << std::get<std::string>(value) << std::endl;
                } else {
                    std::cout << "Key: " << key << ", Value: [unknown type]" << std::endl;
                }
            }
        }
        
        // interactive tick
        double dt = time::Tick();
        // user defined update
        // FILL ME
        
        // swap frame shown
        window::SwapFrame(w);
    }

    // deinit
    if (Quit() != 0) {
        MT_ENGINE_CRITICAL("App::Shutdown(): Failed to initialise marathon. :(");
        return 1;
    }
};
