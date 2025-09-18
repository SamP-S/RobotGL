#include "core/i_system.hpp"

namespace marathon {

// Constructor
ISystem::ISystem(SystemType type, const std::string& name)
    : _name(name), _type(type) {}

// Get the name of the module
std::string ISystem::GetName() const {
    return _name;
}

// Get the type of the module
SystemType ISystem::GetType() const {
    return _type;
}

} // marathon