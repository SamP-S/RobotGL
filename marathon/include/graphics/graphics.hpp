#pragma once

// internal
#include "core/i_system.hpp"

namespace marathon {

namespace graphics {

// system interface
bool Init(BackendFlags flags);
void Quit();

// graphics API facade
void Clear();
void SetClearColour(float r, float g, float b, float a);

} // graphics

} // marathon
