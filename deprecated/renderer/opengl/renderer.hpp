#pragma once

#include <algorithm>
#include <unordered_map>

// include opengl deps
#define GL_VERSION_4_4
#include <GL/glew.h>

#include "renderer/mesh.hpp"
#include "renderer/shader.hpp"
#include "renderer/material.hpp"
#include "renderer/renderer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

class Renderer : public renderer::Renderer {
protected:

    // build-in shaders
    static const std::vector<std::string> s_reservedUniforms;
    static const std::string s_globalHeader;
    static const std::string s_vertexHeader;
    static const std::string s_fragmentHeader;

    /// ---- User Object Handling ---
    /// TODO: implement InternalHandler as a base struct
    struct MeshHandler {
        // hold reference to user struct
        std::shared_ptr<Mesh> mesh = nullptr;
        // opengl internal

        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ibo = 0;
        // error state info
        std::string warnings = "";
        bool isValid = false;
    };

    struct ShaderHandler {
        // hold reference to user struct
        std::shared_ptr<Shader> shader = nullptr;
        // opengl internal
        GLuint program = 0;
        // error state info
        std::string warnings = "";
        bool isValid = false;
    };
    
    /// OpenGL Enum Lookup Maps
    static const std::unordered_map<VertexAttributeFormat, GLenum> s_vertexAttrFormatMap;
    static const std::unordered_map<IndexFormat, GLenum> s_indexFormatMap;
    static const std::unordered_map<PrimitiveType, GLenum> s_primitiveMap;
    static const std::unordered_map<CullFace, GLenum> s_cullFaceMap;
    static const std::unordered_map<CullWinding, GLenum> s_cullWindingMap;
    static const std::unordered_map<DepthFunc, GLenum> s_depthFuncMap;

    /// TODO:
    // should hold default meshes for standard draws calls
    // e.g. circle, square, cube, sphere, etc.
    /// CONSIDER: what arguments should be used for draw calls i.e. 2D vs 3D

    LA::mat4 _projection = LA::mat4();
    LA::mat4 _view = LA::mat4();
    std::stack<LA::mat4> _transforms;
    /// TODO: consider alternative to raw ptr, could use int idxer
    /// also move to RenderState
    ShaderHandler* _shaderHandler = nullptr;


    /// TODO: replace vectors with ordered map
    std::vector<MeshHandler> _meshHandlers;
    std::vector<ShaderHandler> _shaderHandlers;

    /// internal user struct handler methods
    int CreateShaderHandler(std::shared_ptr<Shader> shader);
    int FindOrCreateShaderHandler(std::shared_ptr<Shader> shader);

    int CreateMeshHandler(std::shared_ptr<Mesh> mesh);
    int FindOrCreateMeshHandler(std::shared_ptr<Mesh> mesh);
    
    bool SetDefaultUniforms();
    bool SetMaterialUniforms(std::shared_ptr<Material> material);

    // internal opengl error state check methods
    bool CheckError();
    bool CheckFrameBufferError();
    bool CheckShaderError(GLuint shader);
    bool CheckProgramError(GLuint program);

public:
    Renderer();
    ~Renderer();

    // module interface
    bool Boot() override;
    bool Shutdown() override;

    /// --- Validation ---
    bool ValidateShader(std::shared_ptr<Shader> shader, std::string& err_msg) override;
    bool ValidateMesh(std::shared_ptr<Mesh> mesh, std::string& err_msg) override;

    /// --- Drawing ---
    // clear active canvas/screen of all color, depth, and stencil buffers
    void Clear() override;
    // clear active canvas/screen according to bools args
    void Clear(bool clearColor, bool clearStencil, bool clearDepth) override;

    /// --- Draw Calls ---
    void Draw(std::shared_ptr<Mesh> mesh) override;

    /// --- State Management ---
    void SetState(RendererState state) override;
    void ResetState() override;
    bool IsUsable() override;
    // colour
    LA::vec4 GetClearColor() override;
    LA::vec4 GetColorMask() override;
    void SetClearColor(const LA::vec4& colour) override;
    void SetColorMask(const LA::vec4& mask) override;
    // culling
    bool GetCullTest() override;
    CullFace GetCullFace() override;
    CullWinding GetCullWinding() override;
    void SetCullTest(bool enabled) override;
    void SetCullFace(CullFace face) override;
    void SetCullWinding(CullWinding winding) override;
    // depth triangle vertices we're defining them in a certain winding order that is either clockwise or counter-clockwise. Each triangle consists of 3 vertices and we specify those 3 vertices in a winding order as seen from the center of the triangle.
    bool GetDepthTest() override;
    DepthFunc GetDepthFunction() override;
    bool GetDepthMask() override;
    void SetDepthTest(bool enabled) override;
    void SetDepthFunction(DepthFunc func) override;
    void SetDepthMask(bool enabled) override;
    // rasterization
    float GetLineWidth() override;
    float GetPointSize() override;
    bool GetIsWireframe() override;
    void SetLineWidth(float width) override;
    void SetPointSize(float size) override;
    void SetIsWireframe(bool enabled) override;
    // bound active objects
    std::shared_ptr<renderer::Shader> GetShader() override;
    void SetShader(std::shared_ptr<renderer::Shader> shader) override;

    /// --- Shader Methods ---
    bool HasUniform(const std::string& key) override;

    // single value uniforms
    bool SetUniform(const std::string& key, int value) override;
    bool SetUniform(const std::string& key, uint32_t value) override;
    bool SetUniform(const std::string& key, float value) override;
    bool SetUniform(const std::string& key, double value) override;
    // vector uniforms
    bool SetUniform(const std::string& key, const LA::vec2& v) override;
    bool SetUniform(const std::string& key, float x, float y) override;
    bool SetUniform(const std::string& key, const LA::vec3& v) override;
    bool SetUniform(const std::string& key, float x, float y, float z) override;
    bool SetUniform(const std::string& key, const LA::vec4& v) override;
    bool SetUniform(const std::string& key, float x, float y, float z, float w) override;
    // matrix uniforms
    bool SetUniform(const std::string& key, const LA::mat2& m) override;
    bool SetUniform(const std::string& key, const LA::mat3& m) override;
    bool SetUniform(const std::string& key, const LA::mat4& m) override;
    // uniform property
    bool SetUniform(const std::string& key, const UniformProperty& value) override;

};

} // opengl

} // renderer

} // marathon
