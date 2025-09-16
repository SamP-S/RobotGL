#include "core/object.hpp"

namespace marathon {

Object::Object(const std::string& type)
    : _mType(type) {}

UUID Object::GetUUID() const { return _mUUID; }
std::string Object::GetName() const { return _mName; }
void Object::SetName(const std::string& name) { _mName = name; }
std::string Object::GetType() const { return _mType; }

} // marathon