#pragma once

// PUBLIC HEADER

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <cstdlib>
#include <unordered_map>
#include <array>
#include <stdexcept>
#include <cstring>

#include "la_extended.h"
#include "core/resource.hpp"
#include "renderer/material.hpp"

namespace marathon {

namespace renderer {

enum class PrimitiveType {
    INVALID,
    TRIANGLES,
    FAN,
    STRIP
};

enum class BufferFormat {
    INVALID,
    // floating
    HALF_FLOAT, // 16-bit
    FLOAT,      // 32-bit
    DOUBLE,     // 64-bit
    // signed
    INT8, 
    INT16,
    INT32,
    // unsigned
    UINT8,
    UINT16,
    UINT32
};

enum class VertexAttributeFormat {
    // error
    INVALID,
    // floating
    HALF_FLOAT, // 16-bit
    FLOAT,      // 32-bit
    DOUBLE,     // 64-bit
    // signed
    INT8, 
    INT16,
    INT32,
    // unsigned
    UINT8,
    UINT16,
    UINT32
};

enum class IndexFormat {
    INVALID,
    UINT8,
    UINT16,
    UINT32
};

enum class VertexAttribute {
    INVALID,
    POSITION,
    NORMAL,
    TANGENT,
    COLOUR,
    TEXCOORD0,
    TEXCOORD1,
    TEXCOORD2,
    TEXCOORD3
};

enum class DataDirty {
    INVALID,
    DIRTY_REALLOC,
    DIRTY_UPDATE,
    DIRTY_DELETE,
    CLEAN
};

/// Vertex attribute descriptor for mesh vertex data layout
/// describes a single vertex attribute
struct VertexAttributeDescriptor {
    VertexAttribute attribute = VertexAttribute::POSITION;
    int numComponents = 3;
    VertexAttributeFormat format = VertexAttributeFormat::FLOAT;
};

/// TODO:
/// allow for multiple vertex buffers/data streams rather than a single interleaved vbo
/// allow for buffer usage to be set (currently STATIC only)
/// should centralise all raw data handling into a single buffer class
/// replace void* with c++ standard alternatives e.g. std::array for runtime allocated fixed array or std::vector for dynamicly sized array

/// TODO: 
/// - add bound calculations for physics to use
/// - add instancing support
/// - implement support for multiple vbos
/// - add support for setting buffer usage (currently STATIC only)
/// - add validation on data being input, accepts absolute shit atm, throw errors for the factor to catch
/// - additionally add flags to allow for data validation checks to be skipped for performance
/// - improve performance by using ptr array fills for gets and passing const references
/// - add skeleton bones and stuff
/// - add support for optional data normalisation, currently user must normalise data if required

/// CONSIDER: allow for usage to be set per stream rather than per mesh

class Mesh : public Resource {
protected:
    /// --- VBO ---
    // buffer
    void* _vertexData = nullptr; 
    DataDirty _vertexDataDirty = DataDirty::CLEAN;
    // vertex params
    int _vertexCount = 0;
    std::vector<VertexAttributeDescriptor> _vertexAttributeDescriptors = {};
    // vertex maps
    static const std::unordered_map<VertexAttributeFormat, size_t> s_vertexAttributeFormatMap;
    static const std::unordered_map<VertexAttribute, int> s_vertexAttributeLayoutMap;

    /// --- IBO ---
    // buffer
    void* _indexData = nullptr;
    DataDirty _indexDataDirty = DataDirty::CLEAN;
    // index params
    int _indexCount = 0;
    IndexFormat _indexFormat = IndexFormat::UINT16;
    PrimitiveType _primitive = PrimitiveType::TRIANGLES;
    // index maps
    static const std::unordered_map<IndexFormat, size_t> s_indexFormatMap;


    // clear all data
    void Clear();

    /// --- VBO ---
    // release memory and reset params
    void ClearVertices();
    // call to stop data being uploaded to GPU next frame
    void ClearVertexDirtyFlag();
    // INTERNAL get attribute index in descriptor list
    int GetVertexAttributeIndex(VertexAttribute attr) const;
    // allocates memory according to attributes, vertex data expected to be interleaved in provided order
    void SetVertexParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes);
    // will error if size/offset data range outside expected
    void SetVertexData(void* data, size_t size, size_t src_start, size_t dest_start);


    /// --- IBO ---
    // release memory and reset params
    void ClearIndices();
    // call to stop data being uploaded to GPU next frame
    void ClearIndexDirtyFlag();
    // set index data formatting
    void SetIndexParams(int indexCount, IndexFormat format, PrimitiveType primitive);
    // will error if size/offset data range outside expected
    // use SetIndexParams to reallocate buffer size if needed
    void SetIndexData(void* data, size_t size, size_t src_start, size_t dest_start);

    /// --- Material ---
    std::shared_ptr<Material> _material = nullptr;
    
public:
    // create empty mesh
    Mesh();
    ~Mesh();

    // vbo getters
    const void* GetVertexPtr() const;
    int GetVertexCount() const;
    std::vector<VertexAttributeDescriptor> GetVertexAttributes() const;
    DataDirty GetVertexDirtyFlag() const;
    bool HasVertexAttribute(VertexAttribute attr) const;
    int GetVertexAttributeLocation(VertexAttribute attr) const;
    int GetVertexAttributeComponents(VertexAttribute attr) const;
    VertexAttributeFormat GetVertexAttributeFormat(VertexAttribute attr) const;
    size_t GetVertexAttributeOffset(VertexAttribute attr) const;
    size_t GetVertexSize() const;

    // ibo getters
    const void* GetIndexPtr() const;
    int GetIndexCount() const;
    size_t GetIndexSize() const;
    IndexFormat GetIndexFormat() const;
    PrimitiveType GetPrimitiveType() const;
    DataDirty GetIndexDirtyFlag() const;

    // material
    std::shared_ptr<Material> GetMaterial() const;
    void SetMaterial(std::shared_ptr<Material> material);
};

/// TODO: implement mesh subdivision

class BoxMesh : public Mesh {
protected:
    LA::vec3 _size = {1.0f, 1.0f, 1.0f};

    void Generate();

public:
    BoxMesh();
    ~BoxMesh();

    LA::vec3 GetSize() const;
    void SetSize(LA::vec3 size);
};

class QuadMesh : public Mesh {
protected:
    LA::vec2 _size = {1.0f, 1.0f};

    void Generate();

public:
    QuadMesh();
    ~QuadMesh();

    LA::vec2 GetSize() const;
    void SetSize(LA::vec2 size);
};

class PlaneMesh : public Mesh {
protected:
    LA::vec2 _size = {1.0f, 1.0f};

    void Generate();

public:
    PlaneMesh();
    ~PlaneMesh();

    LA::vec2 GetSize() const;
    void SetSize(LA::vec2 size);
};

class SphereMesh : public Mesh {
protected:
    float _radius = 0.5f;
    int _latSegments = 16;
    int _longSegments = 16;

    void Generate();

public:
    SphereMesh();
    ~SphereMesh();

    float GetRadius() const;
    void SetRadius(float radius);
    int GetLatitudeSegments() const;
    int GetLongitudeSegments() const;
    void SetLatitudeSegments(int latSegments);
    void SetLongitudeSegments(int longSegments);
    void SetSegments(int latSegments, int longSegments);
};

class RawMesh : public Mesh {
public:
    RawMesh();
    ~RawMesh();

    // mesh operations
    using Mesh::Clear;

    // vbo
    using Mesh::ClearVertices;
    using Mesh::ClearVertexDirtyFlag;
    using Mesh::SetVertexParams;
    using Mesh::SetVertexData;

    // ibo
    using Mesh::ClearIndices;
    using Mesh::ClearIndexDirtyFlag;
    using Mesh::SetIndexParams;
    using Mesh::SetIndexData;
};


} // renderer

} // marathon

