#include "core/i_system.hpp"

namespace marathon {

// Constructor
ISystem::ISystem(SystemID type, const std::string& name)
    : _name(name), _sys_id(type) {}

// Get the name of the module
std::string ISystem::GetName() const {
    return _name;
}

// Get the type of the module
SystemID ISystem::GetSystemID() const {
    return _sys_id;
}

} // marathon