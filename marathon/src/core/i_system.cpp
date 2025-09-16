#include "core/i_system.hpp"

namespace marathon {

// Constructor
ISystem::ISystem(SystemType type, const std::string& name)
    : _name(name), _type(type), _active(false) {}

// Destructor
ISystem::~ISystem() {}

// Get if the module has been booted
bool ISystem::IsActive() const {
    return _active;
}

// Get the name of the module
std::string ISystem::GetName() const {
    return _name;
}

// Get the type of the module
SystemType ISystem::GetType() const {
    return _type;
}

} // namespace marathon