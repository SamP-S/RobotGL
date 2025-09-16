#pragma once

#include <string>
#include <map>
#include <unordered_map>

namespace marathon {

// as enum for implicit int conversion
enum ModuleType {
    UNKNOWN = -1,
    WINDOW,
    INPUT,
    MATHS,
    RENDERER,
    AUDIO,
    EVENTS,
    TIME,
    MODULE_MAX_ENUM
};

//// TODO:
// Should probably keep modules in a registry together so all modules can access one another on the backend without making access public to fundamental module objects

class Module {
public:
    Module(ModuleType mType, const std::string& name);
    virtual ~Module();

    // standard methods for safe booting and shutting down
    // allows for modules to be instanced before booting in the correct order
    virtual bool Boot() = 0;
    virtual bool Shutdown() = 0;
    
    bool IsActive() const;
    std::string GetName() const;
    ModuleType GetType() const;

    // template <typename T>
    // static T* GetInstance(ModuleType mType) {
    //     if (mType == ModuleType::UNKNOWN)
    //         return nullptr;
    //     return nullptr;
    // }

protected:
    // active should be set by derived classes during boot/shutdown
    bool _active = false;

private:
    // name and type of module are fixed on construction
    std::string _name = "unknown";
    ModuleType _mType = ModuleType::UNKNOWN;

    // static std::shared_ptr<Module> _instances[MODULE_MAX_ENUM];
    // static void RegisterInstance(std::shared_ptr<Module> instance);

};

} // marathon
