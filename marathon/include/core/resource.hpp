#pragma once

#include <string>

#include "core/object.hpp"

namespace marathon {

struct Resource : public Object {
private:
    std::string _mPath = "./";

public:
    Resource(const std::string& type);
    ~Resource() = default;

};

} // marathon