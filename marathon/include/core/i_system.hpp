#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace marathon {

// as enum for implicit int conversion
enum SystemID {
    SYS_UNKNOWN = -1,
    SYS_AUDIO,
    SYS_EVENTS,
    SYS_GRAPHICS,
    SYS_INPUT,
    SYS_MATHS,
    SYS_TIME,
    SYS_WINDOW,
    SYS_MAX_ENUM
};

class ISystem {
public:
    ISystem(SystemID sys, std::vector<SystemID> deps, const std::string& name);
    virtual ~ISystem() = default;

    // standard methods for safe booting and shutting down
    virtual bool Init() = 0;
    virtual void Quit() = 0;
    
    // getters
    bool IsActive() const;
    std::string GetName() const;
    SystemID GetSystemID() const;
    std::vector<SystemID> GetSystemDeps() const;

private:
    // name and type of system are hardcoded
    bool _active = false;
    std::string _name = "marathon.unknown";
    SystemID _sys_id = SystemID::SYS_UNKNOWN;
    std::vector<SystemID> _sys_deps = {};
};

} // namespace marathon
