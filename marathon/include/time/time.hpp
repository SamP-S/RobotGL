#pragma once

// internal
#include "core/i_system.hpp"

namespace marathon {

namespace time {

// system interface
bool Init();
void Quit();

// time interface
double Tick();
double Time();

} // time

} // marathon