#include "core/i_system.hpp"

namespace marathon {

// Constructor
ISystem::ISystem(SystemID type, std::vector<SystemID> deps, const std::string& name)
    : _active(false), _name(name), _sys_id(type), _sys_deps(std::move(deps)) {}

// Get whether the system is active
bool ISystem::IsActive() const {
    return _active;
}

// Get the name of the system
std::string ISystem::GetName() const {
    return _name;
}

// Get the type of the system
SystemID ISystem::GetSystemID() const {
    return _sys_id;
}

// Get the dependencies of the system
std::vector<SystemID> ISystem::GetSystemDeps() const {
    return _sys_deps;
}

} // marathon