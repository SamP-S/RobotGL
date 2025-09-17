#pragma once

#include <string>

#include "core/object.hpp"

namespace marathon {

namespace core {

struct Resource : public Object {
private:
    std::string _mPath = "./";

public:
    Resource(const std::string& type);
    ~Resource() = default;

};

} // core

} // marathon