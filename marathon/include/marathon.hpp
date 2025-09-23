#pragma once

// std lib
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

// internal
#include "core/backends.hpp"
#include "core/i_system.hpp"

namespace marathon {

bool Impl(BackendFlags flags = SDL2 | OPENGL);
bool Init();
void Quit();

} // marathon
