#pragma once

namespace marathon {

namespace time {

// system interface
bool Init(BackendFlags flags);
void Quit();

// time interface
double Tick();
double Time();

} // time

} // marathon