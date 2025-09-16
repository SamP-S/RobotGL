#pragma once

#include <string>
#include <chrono>

#include "core/i_system.hpp"

namespace marathon {

namespace time {

bool Init();
bool Quit();

double Tick();
double GetDeltaTime();
double GetTime();
double GetFPS();

} // time

} // marathon