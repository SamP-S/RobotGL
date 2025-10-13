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

/// --- MEMORY --- ///
// generic memory store
struct Memory {
    Memory() = delete;  // no default constructor (REQUIRE EXTERNAL FACTORY)
    void* ptr;          // generic pointer
    uint32_t size;      // data size in bytes
};

// Allocate raw memory buffer
const Memory* Alloc(uint32_t size);
// Allocate raw memory buffer and copy data into it
const Memory* Copy(const void* data, uint32_t size);
void Release(const Memory* _mem);


/// --- RENDERING --- ///
void Clear(bool clearColour, bool clearDepth, bool clearStencil);
float* GetClearColour();
void SetClearColour(float r, float g, float b, float a);
double GetClearDepth();
void SetClearDepth(double value);
int32_t GetClearStencil();
void SetClearStencil(int32_t value);


/// --- STATE MANAGEMENT --- ///
enum Winding : bool {
    CCW,
    CW
};
enum Feature : int32_t {
    CULL_FACE,
    DEPTH_TEST,
    SCISSOR_TEST,
    STENCIL_TEST
};
void Enable(Feature f);
void Disable(Feature f);
float GetPointSize();
void SetPointSize(float size);
float GetLineWidth();
void SetLineWidth(float width);
Winding GetFrontFace();
void SetFrontFace(Winding w);

/// --- VIEWPORT --- ///
void Viewport(int32_t x, int32_t y, uint32_t width, uint32_t height);


/// --- HANDLES --- ///
typedef uint32_t FrameBufferHandle;
typedef uint32_t IndexBufferHandle;
typedef uint32_t ProgramHandle;
typedef uint32_t ShaderHandle;
typedef uint32_t TextureHandle;
typedef uint32_t UniformHandle;
typedef uint32_t VertexBufferHandle;
typedef uint32_t VertexLayoutHandle;
#define INVALID_HANDLE UINT32_MAX

/// --- VERTEX LAYOUTS --- ///
enum Attrib : int32_t {
    ATTRIB_POSITION,
    ATTRIB_NORMAL,
    ATTRIB_TANGENT,
    ATTRIB_BITANGENT,
    ATTRIB_COLOUR_0,
    ATTRIB_COlOUR_1,
    ATTRIB_COLOUR_2,
    ATTRIB_COLOUR_3,
    ATTRIB_INDICES,
    ATTRIB_WEIGHT,
    ATTRIB_TEX_COORD_0,
    ATTRIB_TEX_COORD_1,
    ATTRIB_TEX_COORD_2,
    ATTRIB_TEX_COORD_3,
    ATTRIB_TEX_COORD_4,
    ATTRIB_TEX_COORD_5,
    ATTRIB_TEX_COORD_6,
    ATTRIB_TEX_COORD_7,
    ATTRIB_COUNT
};

enum AttribType : int32_t {
    INT8,
    INT16,
    INT32,
    UINT8,
    UINT16,
    UINT32,
    HALF,
    FLOAT,
    DOUBLE
};

struct VertexLayout {
    VertexLayout();
    VertexLayout& Begin();  // support builder design pattern
    void End();
    // add attribute to vertex layout
    VertexLayout& Add(
        Attrib attrib,      // attribute semantic
        uint8_t num,        // num of elements i.e. 1, 2, 3 or 4
        AttribType type,    // element data type
        bool normalised = false    // normalise data to range 0.0 - 1.0
    );
    // skip num bytes in vertex stream
    VertexLayout& Skip(uint8_t num);

    // check layout has attribute
    bool Has(Attrib attrib) const;
    // get attribute relative offset from vertex
    uint16_t GetOffset(Attrib attrib) const;
    // get vertex stride
    uint16_t GetStride() const;
    // get size of vertex buffer for number of vertices
    uint32_t GetSize(uint32_t num) const;

private:
    // total vertex stride
    uint16_t _stride;
    // calculated relative attribute positions
    uint16_t _offset[ATTRIB_COUNT];
    uint16_t _size[ATTRIB_COUNT];
    // attribute order or -1 if not present
    int8_t _attributes[ATTRIB_COUNT];
    // cached attribute properties
    uint8_t _nums[ATTRIB_COUNT];
    AttribType _types[ATTRIB_COUNT];
    bool _normalised[ATTRIB_COUNT];
};

VertexLayoutHandle CreateVertexLayout(const VertexLayout& layout);
void DestroyVertexLayout(VertexLayoutHandle handle);

/// --- VERTEX/INDEXED BUFFERS --- ///
/// TODO: flags for dynamic
// draw methods:
// void SetVertexBuffer(uint8_t stream, VertexBufferHandle handle);
// void SetVertexBuffer(uint8_t stream, VertexBufferHandle handle, uint32_t startVertex, uint32_t numVertex, VertexLayoutHandle layoutHandle);

enum BufferFlags : int32_t {
    BUFFER_NONE         = 0x0000,
    BUFFER_ALLOW_RESIZE = 0x0001,
    BUFFER_INDEX32      = 0x0002
    /// TODO: support more buffer features
};

VertexBufferHandle CreateVertexBuffer(
    const Memory* mem,
    const VertexLayout& layout,
    uint32_t flags = BUFFER_NONE
);
void SetName(
    VertexBufferHandle handle,
    const char* name    // zero-terminated str
);
void DestroyVertexBuffer(VertexBufferHandle handle);

IndexBufferHandle CreateIndexBuffer(
    const Memory* mem,
    uint32_t flags = BUFFER_NONE
);
void SetName(
    VertexBufferHandle handle,
    const char* name    // zero-terminated str
);
void DestroyIndexBuffer(IndexBufferHandle handle);



/// --- SHADERS / PROGRAMS --- ///
// enum ShaderType : int32_t {
//     VERTEX_SHADER,
//     FRAGRMENT_SHADER,
//     /// TODO:
//     // implement more
// };

ShaderHandle CreateShader(const Memory* mem);
uint16_t GetShaderUniforms(
    ShaderHandle handle,
    UniformHandle* uniforms = nullptr,
    uint16_t max = 0
);
void SetName(
    ShaderHandle handle,
    const char* name    // zero-terminated c-str
);
void DestroyShader(ShaderHandle handle);

ProgramHandle CreateProgram(
    ShaderHandle vert,
    ShaderHandle frag
);
/// TODO: compute shader
void DestroyProgram(ProgramHandle handle);


/// --- EXECUTION --- ///
enum DiscardFlags : uint8_t {
    DISCARD_NONE               = 0x00,
    DISCARD_BINDINGS           = 0x01,
    DISCARD_INDEX_BUFFER       = 0x02,
    DISCARD_INSTANCE_DATA      = 0x04,
    DISCARD_STATE              = 0x08,
    DISCARD_TRANSFORM          = 0x10,
    DISCARD_VERTEX_STREAMS     = 0x20,
    DISCARD_ALL                = 0xff,
};

// discard all previously set state for draw call
void Discard();

// submit an empty primite for rendering
// applies uniforms and draw state but no geometry
// e.g. clear screen
void Touch();

// submit primitive for rendering
void Submit(
    ProgramHandle program,
    uint32_t depth = 0
);

/// --- CONTEXT --- ///
/// TODO: textures & texture bindings
#define MAX_VERTEX_STREAM 4

struct Stream {
    void Clear() {
        startVertex     = 0;
        handle          = INVALID_HANDLE;
        layoutHandle    = INVALID_HANDLE;
    }

    uint32_t            startVertex;
    VertexBufferHandle  handle;
    VertexLayoutHandle  layoutHandle;
};

struct RenderDraw {
    void Clear(uint8_t flags = DISCARD_ALL) {
        if ((flags & DISCARD_STATE) != 0) {
            /// TODO:
        }
        if ((flags & DISCARD_TRANSFORM) != 0) {
            /// TODO:
        }
        if ((flags & DISCARD_INSTANCE_DATA) != 0) {
            /// TODO:
        }
        if ((flags & DISCARD_VERTEX_STREAMS) != 0) {
            numVertices = UINT32_MAX;
            streamMask = 0;
            stream[0].Clear();
        }
        if ((flags & DISCARD_INDEX_BUFFER) != 0) {
            startIndex = 0;
            numIndices = 0;
            indexBuffer = INVALID_HANDLE;
            submitFlags = 0;
        }

        /// TODO:

    }

    Stream stream[MAX_VERTEX_STREAM];
    uint32_t startIndex;
    uint32_t numIndices;
    uint32_t numVertices;
    uint8_t  submitFlags;
    uint8_t  streamMask;

    IndexBufferHandle indexBuffer;
};

} // graphics

} // marathon
