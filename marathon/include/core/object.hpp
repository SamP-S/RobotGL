#pragma once

#include <string>

#include "core/uuid.hpp"

namespace marathon {

/// An object is a base class for all instanitable classes (resources & nodes)
/// TODO: implement UUID from file, UUID currently copies during copy
class Object {
private:
    std::string _mType;
    std::string _mName = "default_name";
    UUID _mUUID = UUID();

public:
    Object(const std::string& type="marathon.object");
    ~Object() = default;

    UUID GetUUID() const;
    std::string GetName() const;
    void SetName(const std::string& name);
    std::string GetType() const;
};

} // marathon
