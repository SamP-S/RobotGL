#include "renderer/mesh.hpp"

#include "core/logger.hpp"

namespace marathon {

namespace renderer {


/// --- INTERNAL ---
/// TODO: Implement a dictionary to cache indexes instead of linear search everytime for big speed improvement
// find index of attribute in descriptor list or return -1 if not found
int Mesh::GetVertexAttributeIndex(VertexAttribute attr) const {
    for (int i = 0; i < _vertexAttributeDescriptors.size(); i++) {
        if (_vertexAttributeDescriptors[i].attribute == attr) {
            return i;
        }
    }
    return -1;
}



/// --- VBO ---
const std::unordered_map<VertexAttributeFormat, size_t> Mesh::s_vertexAttributeFormatMap = {
    {VertexAttributeFormat::INVALID, 0},
    {VertexAttributeFormat::HALF_FLOAT, 2},
    {VertexAttributeFormat::FLOAT, 4},
    {VertexAttributeFormat::DOUBLE, 8},
    {VertexAttributeFormat::INT8, 1},
    {VertexAttributeFormat::INT16, 2},
    {VertexAttributeFormat::INT32, 4},
    {VertexAttributeFormat::UINT8, 1},
    {VertexAttributeFormat::UINT16, 2},
    {VertexAttributeFormat::UINT32, 4}
};

const std::unordered_map<VertexAttribute, int> Mesh::s_vertexAttributeLayoutMap = {
    {VertexAttribute::INVALID, -1},
    {VertexAttribute::POSITION, 0},
    {VertexAttribute::NORMAL, 1},
    {VertexAttribute::TANGENT, 2},
    {VertexAttribute::COLOUR, 3},
    {VertexAttribute::TEXCOORD0, 4},
    {VertexAttribute::TEXCOORD1, 5},
    {VertexAttribute::TEXCOORD2, 6},
    {VertexAttribute::TEXCOORD3, 7}
};

void Mesh::ClearVertices() {
    // Empty implementation
    /// TODO: Implement
    MT_CORE_DEBUG("src/renderer/mesh.cpp: Mesh::ClearVertices() not implemented");
}

void Mesh::ClearVertexDirtyFlag() {
    _vertexDataDirty = DataDirty::CLEAN;
}

const void* Mesh::GetVertexPtr() const {
    return _vertexData;
}

int Mesh::GetVertexCount() const {
    return _vertexCount;
}

std::vector<VertexAttributeDescriptor> Mesh::GetVertexAttributes() const {
    return _vertexAttributeDescriptors;
}

DataDirty Mesh::GetVertexDirtyFlag() const {
    return _vertexDataDirty;
}

// return false if failed to find attribute
bool Mesh::HasVertexAttribute(VertexAttribute attr) const {
    return GetVertexAttributeIndex(attr) != -1;
}

// returns -1 if failed
int Mesh::GetVertexAttributeLocation(VertexAttribute attr) const {
    auto it = s_vertexAttributeLayoutMap.find(attr);
    if (it != s_vertexAttributeLayoutMap.end()) {
        return it->second;
    } else {
        return -1;
    }
}

// return 0 if failed
int Mesh::GetVertexAttributeComponents(VertexAttribute attr) const {
    int idx = GetVertexAttributeIndex(attr);
    if (idx == -1) 
        return 0;
    else
        return _vertexAttributeDescriptors.at(idx).numComponents;
}

// return VertexAttributeFormat::INVALID if failed
VertexAttributeFormat Mesh::GetVertexAttributeFormat(VertexAttribute attr) const {
    int idx = GetVertexAttributeIndex(attr);
    if (idx == -1) 
        return VertexAttributeFormat::INVALID;
    else
        return _vertexAttributeDescriptors.at(idx).format;
}

// return 0 if failed
size_t Mesh::GetVertexAttributeOffset(VertexAttribute attr) const {
    int idx = GetVertexAttributeIndex(attr);
    if (idx == -1) {
        return 0;
    }

    size_t offset = 0;
    for (int i = 0; i < idx; i++) {
        offset += s_vertexAttributeFormatMap.at(_vertexAttributeDescriptors[i].format) * _vertexAttributeDescriptors[i].numComponents;
    }
    return offset;
}

size_t Mesh::GetVertexSize() const {
    size_t size = 0;
    for (int i = 0; i < _vertexAttributeDescriptors.size(); i++) {
        size += s_vertexAttributeFormatMap.at(_vertexAttributeDescriptors[i].format) * _vertexAttributeDescriptors[i].numComponents;
    }
    return size;
}

void Mesh::SetVertexParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes) {
    MT_CORE_DEBUG("Mesh::SetVertexParams(): vertex_count = {0}, attribute_count = {1}", vertexCount, attributes.size());
    ClearVertices();
    _vertexCount = vertexCount;
    _vertexAttributeDescriptors = attributes;
    _vertexData = malloc(GetVertexSize() * vertexCount);
    _vertexDataDirty = DataDirty::DIRTY_REALLOC;
}

void Mesh::SetVertexData(void* data, size_t size, size_t src_start, size_t dest_start) {
    size_t vertexSize = GetVertexSize();
    size_t dataSize = vertexSize * _vertexCount;
    // catch fucky wuckys
    if (data == nullptr) {
        MT_ENGINE_WARN("Mesh::SetVertexData(): data is nullptr");
        return;
    } else if (_vertexData == nullptr) {
        MT_ENGINE_WARN("Mesh::SetVertexData(): vertex data is nullptr");
        return;
    } else if (size + src_start > dataSize - dest_start) {
        MT_ENGINE_WARN("Mesh::SetVertexData(): data range out of bounds");
        return;
    }
    // copy data
    memcpy(_vertexData + dest_start, data + src_start, size);
    // realloc takes precident over update
    if (_vertexDataDirty != DataDirty::DIRTY_REALLOC) 
        _vertexDataDirty = DataDirty::DIRTY_UPDATE;
}



/// --- IBO --- ///
const std::unordered_map<IndexFormat, size_t> Mesh::s_indexFormatMap = {
    {IndexFormat::INVALID, 0},
    {IndexFormat::UINT8, 1},
    {IndexFormat::UINT16, 2},
    {IndexFormat::UINT32, 4}
};

void Mesh::ClearIndices() {
    /// TODO: implement
    MT_CORE_DEBUG("Mesh::ClearIndices() not implemented");
}

void Mesh::ClearIndexDirtyFlag() {
    _indexDataDirty = DataDirty::CLEAN;
}

const void* Mesh::GetIndexPtr() const {
    return _indexData;
}

int Mesh::GetIndexCount() const {
    return _indexCount;
}

size_t Mesh::GetIndexSize() const {
    return s_indexFormatMap.at(_indexFormat);
}

DataDirty Mesh::GetIndexDirtyFlag() const {
    return _indexDataDirty;
}

IndexFormat Mesh::GetIndexFormat() const {
    return _indexFormat;
}

PrimitiveType Mesh::GetPrimitiveType() const {
    return _primitive;
}

void Mesh::SetIndexParams(int indexCount, IndexFormat format, PrimitiveType primitive) {
    MT_CORE_DEBUG("Mesh::SetIndexParams(): index_count = {0}", indexCount);
    ClearIndices();
    _indexCount = indexCount;
    _indexFormat = format;
    _primitive = primitive;
    _indexData = malloc(s_indexFormatMap.at(format) * indexCount);
    _indexDataDirty = DataDirty::DIRTY_REALLOC;
}

void Mesh::SetIndexData(void* data, size_t size, size_t src_start, size_t dest_start) {
    size_t dataSize = s_indexFormatMap.at(_indexFormat) * _indexCount;
    // catch bad args
    if (data == nullptr) {
        MT_ENGINE_WARN("Mesh::SetIndexData() data is nullptr");
        return;
    } else if (_indexData == nullptr) {
        MT_ENGINE_WARN("Mesh::SetIndexData() index data is nullptr");
        return;
    } else if (size + src_start > dataSize - dest_start) {
        MT_ENGINE_WARN("Mesh::SetIndexData() data range out of bounds");
        return;
    }
    memcpy(_indexData + dest_start, data + src_start, size);
    // realloc takes precident over update
    if (_indexDataDirty != DataDirty::DIRTY_REALLOC) 
        _indexDataDirty = DataDirty::DIRTY_UPDATE;

}



/// --- MESH --- ///

Mesh::Mesh() 
    : Resource("marathon.renderer.mesh") {}
Mesh::~Mesh() {}

void Mesh::Clear() {
    ClearVertices();
    ClearIndices();
    _indexDataDirty = DataDirty::DIRTY_DELETE;
}

std::shared_ptr<Material> Mesh::GetMaterial() const {
    return _material;
}

void Mesh::SetMaterial(std::shared_ptr<Material> material) {
    _material = material;
}


/// --- BoxMesh --- ///
/// TODO: allow for not always reallocating during generation
BoxMesh::BoxMesh() {
    Generate();
}

BoxMesh::~BoxMesh() {}

LA::vec3 BoxMesh::GetSize() const {
    return _size;
}

void BoxMesh::SetSize(LA::vec3 size) {
    _size = size;
    Generate();
}

void BoxMesh::Generate() {
    // setup vertex data format
    std::vector<VertexAttributeDescriptor> attributes = {
        {VertexAttribute::POSITION, 3, VertexAttributeFormat::FLOAT},
        {VertexAttribute::NORMAL, 3, VertexAttributeFormat::FLOAT},
        {VertexAttribute::TEXCOORD0, 2, VertexAttributeFormat::FLOAT}
    };
    SetVertexParams(24, attributes);

    float half_x = _size.x / 2.0f;
    float half_y = _size.y / 2.0f;
    float half_z = _size.z / 2.0f;
    float vertices[] = {
        // Positions                   // Normals           // TexCoords
        -half_x, -half_y, -half_z,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         half_x, -half_y, -half_z,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         half_x,  half_y, -half_z,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -half_x,  half_y, -half_z,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        -half_x, -half_y,  half_z,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         half_x, -half_y,  half_z,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         half_x,  half_y,  half_z,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -half_x,  half_y,  half_z,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        -half_x,  half_y,  half_z, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -half_x,  half_y, -half_z, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -half_x, -half_y, -half_z, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -half_x, -half_y,  half_z, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

         half_x,  half_y,  half_z,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         half_x,  half_y, -half_z,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         half_x, -half_y, -half_z,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         half_x, -half_y,  half_z,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        -half_x, -half_y, -half_z,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         half_x, -half_y, -half_z,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         half_x, -half_y,  half_z,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -half_x, -half_y,  half_z,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

        -half_x,  half_y, -half_z,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         half_x,  half_y, -half_z,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         half_x,  half_y,  half_z,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -half_x,  half_y,  half_z,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };
    SetVertexData((void*)vertices, sizeof(vertices), 0, 0);

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    SetIndexParams(36, IndexFormat::UINT32, PrimitiveType::TRIANGLES);
    SetIndexData(indices, sizeof(indices), 0, 0);
}



/// --- QuadMesh --- ///
QuadMesh::QuadMesh() {
    Generate();
}

QuadMesh::~QuadMesh() {}

LA::vec2 QuadMesh::GetSize() const {
    return _size;
}

void QuadMesh::SetSize(LA::vec2 size) {
    _size = size;
    Generate();
}

void QuadMesh::Generate() {
    // setup vertex data format
    std::vector<VertexAttributeDescriptor> attributes = {
        {VertexAttribute::POSITION, 3, VertexAttributeFormat::FLOAT},
        {VertexAttribute::NORMAL, 3, VertexAttributeFormat::FLOAT},
        {VertexAttribute::TEXCOORD0, 2, VertexAttributeFormat::FLOAT}
    };
    SetVertexParams(8, attributes);

    float half_x = _size.x / 2.0f;
    float half_y = _size.y / 2.0f;

    float vertices[] = {
        // Positions             // Normals           // TexCoords
        -half_x, -half_y, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         half_x, -half_y, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         half_x,  half_y, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -half_x,  half_y, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        -half_x, -half_y, 0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         half_x, -half_y, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         half_x,  half_y, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -half_x,  half_y, 0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    };
    SetVertexData((void*)vertices, sizeof(vertices), 0, 0);

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };
    SetIndexParams(12, IndexFormat::UINT32, PrimitiveType::TRIANGLES);
    SetIndexData(indices, sizeof(indices), 0, 0);
}



/// --- PlaneMesh --- ///
PlaneMesh::PlaneMesh() {
    Generate();
}

PlaneMesh::~PlaneMesh() {}

LA::vec2 PlaneMesh::GetSize() const {
    return _size;
}

void PlaneMesh::SetSize(LA::vec2 size) {
    _size = size;
    Generate();
}

void PlaneMesh::Generate() {
    // setup vertex data format
    std::vector<VertexAttributeDescriptor> attributes = {
        {VertexAttribute::POSITION, 3, VertexAttributeFormat::FLOAT},
        {VertexAttribute::NORMAL, 3, VertexAttributeFormat::FLOAT},
        {VertexAttribute::TEXCOORD0, 2, VertexAttributeFormat::FLOAT}
    };
    SetVertexParams(8, attributes);

    float half_x = _size.x / 2.0f;
    float half_y = _size.y / 2.0f;
    float vertices[] = {
        // Positions             // Normals           // TexCoords
        -half_x, 0.0f, -half_y,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         half_x, 0.0f, -half_y,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         half_x, 0.0f,  half_y,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -half_x, 0.0f,  half_y,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

        -half_x, 0.0f, -half_y,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         half_x, 0.0f, -half_y,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         half_x, 0.0f,  half_y,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -half_x, 0.0f,  half_y,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };
    SetVertexData((void*)vertices, sizeof(vertices), 0, 0);

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };
    SetIndexParams(36, IndexFormat::UINT32, PrimitiveType::TRIANGLES);
    SetIndexData(indices, sizeof(indices), 0, 0);
}



/// --- PlaneMesh --- ///
SphereMesh::SphereMesh() {
    Generate();
}

SphereMesh::~SphereMesh() {}

float SphereMesh::GetRadius() const {
    return _radius;
}

void SphereMesh::SetRadius(float radius) {
    if (radius == 0.0f) {
        MT_ENGINE_WARN("SphereMesh::SetRadius(): radius is zero");
    }
    _radius = radius;
    Generate();
}

int SphereMesh::GetLatitudeSegments() const {
    return _latSegments;
}
int SphereMesh::GetLongitudeSegments() const {
    return _longSegments;
}

void SphereMesh::SetLatitudeSegments(int latSegments) {
    SetSegments(latSegments, _longSegments);
}

void SphereMesh::SetLongitudeSegments(int longSegments) {
    SetSegments(_latSegments, longSegments);
}

void SphereMesh::SetSegments(int latSegments, int longSegments) {
    // latitude segments (horizontal)
    if (latSegments < 3) {
        MT_ENGINE_WARN("SphereMesh::SetSegments(): segments is less than 3");
        _latSegments = 3;
    } else {
        _latSegments = latSegments;
    }
    // longitude segments (vertical)
    if (longSegments < 3) {
        MT_ENGINE_WARN("SphereMesh::SetSegments(): segments is less than 3");
        _longSegments = 3;
    } else {
        _longSegments = longSegments;
    }
    Generate();
}


/// NOTE: https://danielsieger.com/blog/2021/03/27/generating-spheres.html
/// TODO: https://danielsieger.com/blog/2021/05/03/generating-primitive-shapes.html
void SphereMesh::Generate() {
        // setup vertex data format
    std::vector<VertexAttributeDescriptor> attributes = {
        {VertexAttribute::POSITION, 3, VertexAttributeFormat::FLOAT}
    };

    std::vector<LA::vec3> vertices;
    std::vector<uint32_t> indices;

    // add top vertex
    vertices.push_back(LA::vec3({0.0f, _radius, 0.0f}));
    // generate iterating by lat -> long
    for (int j = 0; j < _latSegments - 1; j++) {
        double phi = M_PI * (double)(j + 1) / (double)_latSegments;
        for (int i = 0; i < _longSegments; i++) {
            double theta = 2.0 * M_PI * (double)i / (double)_longSegments;
            float x = _radius * sin(phi) * cos(theta);
            float y = _radius * cos(phi);
            float z = _radius * sin(phi) * sin(theta);
            vertices.push_back(LA::vec3({x, y, z}));
        }
    }
    // add bottom vertex
    vertices.push_back(LA::vec3({0.0f, -_radius, 0.0f}));

    // add triangles for top and bottom
    for (int i = 0; i < _longSegments; ++i) {
        // top
        uint32_t i0 = i + 1;
        uint32_t i1 = (i + 1) % _longSegments + 1;
        indices.push_back(0);
        indices.push_back(i1);
        indices.push_back(i0);

        // bottom
        i0 = i + _longSegments * (_latSegments - 2) + 1;
        i1 = (i + 1) % _longSegments + _longSegments * (_latSegments - 2) + 1;
        indices.push_back(vertices.size() - 1);
        indices.push_back(i0);
        indices.push_back(i1);
    }

    // add triangles for quads
    for (int j = 0; j < _latSegments - 2; j++) {
        uint32_t j0 = j * _longSegments + 1;
        uint32_t j1 = (j + 1) * _longSegments + 1;
        for (int i = 0; i < _longSegments; i++) {
            uint32_t i0 = j0 + i;
            uint32_t i1 = j0 + (i + 1) % _longSegments;
            uint32_t i2 = j1 + (i + 1) % _longSegments;
            uint32_t i3 = j1 + i;

            // Add two triangles for each quad
            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }

    SetVertexParams(vertices.size(), attributes);
    SetVertexData((void*)&vertices[0][0], sizeof(LA::vec3) * vertices.size(), 0, 0);

    SetIndexParams(indices.size(), IndexFormat::UINT32, PrimitiveType::TRIANGLES);
    SetIndexData((void*)&indices[0], sizeof(uint32_t) * indices.size(), 0, 0);
}



/// --- RawMesh --- ///
// ensure to call base class constructor
RawMesh::RawMesh()
    : Mesh() {}
// base class destructor is called automatically
RawMesh::~RawMesh() {}


} // namespace renderer

} // namespace marathon