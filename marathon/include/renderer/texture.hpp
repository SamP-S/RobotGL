#pragma once

// PUBLIC HEADER

#include "la_extended.h"
#include "core/resource.hpp"

namespace marathon {

namespace renderer {

/// TODO:
// mipmaps
// support changing texture parameters
// support compressed texture data formats
// depth sampling?

enum class TexType {
    TYPE_1D,
    TYPE_2D,
    TYPE_3D,
    TYPE_CUBE
};

enum class TexFilter {
    NEAREST,
    LINEAR
};

enum class TexWrap {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum class TexPixelFormat {
    NONE,
    // 1 channel
    R_8u,
    // 2 channel
    RG_8u,
    // 3 channel
    RGB_8u,
    RGB_16f,
    RGB_32f,    // HDR
    // 4 channel
    RGBA_8u,
    RGBA_16f,
    RGBA_32f,   // HDR
    // depth/stencil
    DEPTH_24f_STENCIL_8u,
    DEPTH_16f,
    DEPTH_24f,
    DEPTH_32f,
    STENCIL_1u,
    STENCIL_2u,
    STENCIL_8u
};

// forward delcare
class Renderer;

class Texture : public Resource {
protected:
    Texture();
    ~Texture();

    int _width;
    int _height;
    int _channels;
    int _mipmapCounts;
    TexFilter _filter;
    TexFilter _mipmapFilter;
    TexWrap _wrap;
    TexPixelFormat _pixelFormat;

public:
    // Fixed Properties
    int GetWidth();
    int GetHeight();
    int GetChannels();
    LA::vec3 GetDimensions();
    int GetMipmapCount();
    TexPixelFormat GetPixelFormat();

    // Dynamic Properties
    TexFilter GetFilter();
    TexFilter GetMipmapFilter();
    TexWrap GetWrap();
    void SetFilter(TexFilter filter);
    void SetMipmapFilter(TexFilter filter);
    void SetWrap(TexWrap wrap);

    // // Operations
    // void ReloadMipmaps();
};

} // renderer

} // marathon