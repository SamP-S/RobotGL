#pragma once

#include <string>
#include <map>
#include <unordered_map>

namespace marathon {

// as enum for implicit int conversion
enum SystemType {
    SYS_UNKNOWN = -1,
    SYS_WINDOW,
    SYS_INPUT,
    SYS_MATHS,
    SYS_GRAPHICS,
    SYS_AUDIO,
    SYS_EVENTS,
    SYS_TIME,
    SYS_MAX_ENUM
};

enum Backends : int32_t {
    NONE        = 0x0000,
    // gpu api
    OPENGL      = 0x0001,
    VULKAN      = 0x0002,
    RENDERER    = 0x000F,
    // window context
    SDL2        = 0x0010,
    GLFW        = 0x0020,
    WINDOW      = 0x00F0,
    // gui toolkit
    IMGUI       = 0x0100,
    GUI         = 0x0F00,
};
typedef int32_t BackendFlags;


class ISystem {
public:
    ISystem(SystemType mType, const std::string& name);
    virtual ~ISystem() = default;

    // standard methods for safe booting and shutting down
    virtual bool Init(BackendFlags flags) = 0;
    virtual void Quit() = 0;
    
    // getters
    bool IsActive() const;
    std::string GetName() const;
    SystemType GetType() const;

private:
    // name and type of module are hardcoded
    std::string _name = "unknown";
    SystemType _type = SystemType::SYS_UNKNOWN;

};

} // namespace marathon
