#include <vector>

#include "marathon.hpp"

namespace marathon {

    /// TODO: ensure the boot/shutdown orders are correct
    // ensure each module ensures their required modules have already booted successfully
    // e.g. renderer checks window has booted

    /// NOTE: this is the first time instancing the modules but instancing order doesn't matter

    int Init() {
        // maintain specific module order for booting correctly
        std::vector<Module*> modules = {
            &time::Time::Instance(),
            &window::Window::Instance(),
            &renderer::Renderer::Instance(),
            &events::Events::Instance()
        };
        int moduleBootFail = 0;
        for (auto module : modules) {
            if (module != nullptr && !module->Boot())
                moduleBootFail++;
        }
        return moduleBootFail;
    }

    int Quit() {
        // maintain specific module order for shutting down correctly
        std::vector<Module*> modules = {
            &time::Time::Instance(),
            &window::Window::Instance(),
            &renderer::Renderer::Instance(),
            &events::Events::Instance()
        };
        int moduleShutdownFails = 0;
        for (auto module : modules) {
            if (module != nullptr && !module->Shutdown())
                moduleShutdownFails++;
        }
        return moduleShutdownFails;
    }

} // marathon