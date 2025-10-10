#pragma once

// std libs
#include <cstdint>
// internal
#include "core/i_system.hpp"

namespace marathon {

namespace graphics {

// system interface
bool Init();
void Quit();

// graphics API facade

/// --- DOCUMENTATION --- ///
// See docs.gl/gl3

/// --- RENDERING --- ///
void Clear(bool clearColour, bool clearDepth, bool clearStencil);
float* GetClearColour();
void SetClearColour(float r, float g, float b, float a);
double GetClearDepth();
void SetClearDepth(double value);
int32_t GetClearStencil();
void SetClearStencil(int32_t value);

/// --- FRAMEBUFFER --- ///
void CreateFrameBuffers(uint32_t n, uint32_t frameBuffers);


/// --- BUFFER OBJECTS --- ///
void CreateBuffers(uint32_t n, uint32_t* buffers);

/// --- VERTEX ATTRIBUTES --- ///

/// --- STATE MANAGEMENT --- ///

/// --- SHADERS --- ///



} // graphics

} // marathon
