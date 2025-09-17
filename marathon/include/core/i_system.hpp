#pragma once

#include <string>
#include <map>
#include <unordered_map>

namespace marathon {

// as enum for implicit int conversion
enum SystemType {
    UNKNOWN = -1,
    WINDOW,
    INPUT,
    MATHS,
    RENDERER,
    AUDIO,
    EVENTS,
    TIME,
    SYSTEM_MAX_ENUM
};

class ISystem {
public:
    ISystem(SystemType mType, const std::string& name);
    virtual ~ISystem() = default;

    // standard methods for safe booting and shutting down
    virtual bool Init() = 0;
    virtual bool Quit() = 0;
    
    // getters
    bool IsActive() const;
    std::string GetName() const;
    SystemType GetType() const;

protected:
    // active should be set by derived classes during boot/shutdown
    bool _active = false;

private:
    // name and type of module are hardcoded
    std::string _name = "unknown";
    SystemType _type = SystemType::UNKNOWN;

};

} // marathon
