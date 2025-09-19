#pragma once

// PUBLIC HEADER

// std lib
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <stack>

// includes
#include "la_extended.h"
#include "core/module.hpp"
#include "renderer/mesh.hpp"
#include "renderer/material.hpp"
#include "renderer/shader.hpp"

namespace marathon {

namespace renderer {

/// TODO:
// Add TEXTURES, muy importante
/// Add fonts
/// Add Per-Sample Processing:
// - Scissor Test
// - Stencil Test
// - Multisampling / Anti-Aliasing
// Add coordinate space sheering
// Add coordinate space screen->global and global->screen
// Add get system dependant maximums of backend stuff e.g. texture size
// Add lib dependant maximums e.g. file formats etc.
// Add get gpu info?
// Add WAY MORE RENDER STATISTICS
// Implement batch drawing for performance

enum class CullFace {
    FRONT,
    BACK,
    FRONT_AND_BACK
};
enum class CullWinding {
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

enum class DepthFunc {
    NEVER,
    ALWAYS,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL
};

enum class DrawMode {
    POINTS,
    LINES,
    FILL
};

// per frame
struct RenderStats {
    int frameIndex = 0;
    int drawCalls = 0;
    int trianglesRendered = 0;
};

struct RendererState {
    LA::vec4 clearColor = {0.0f, 0.0f, 0.0f, 0.0f};
    LA::vec4 colorMask = {1.0f, 1.0f, 1.0f, 1.0f};
    bool cullTest = false;
    CullFace cullFace = CullFace::BACK;
    CullWinding cullWinding = CullWinding::COUNTER_CLOCKWISE;
    bool depthTest = false;
    DepthFunc depthFunc = DepthFunc::LESS;
    bool depthMask = true;
    float lineWidth = 1.0f;
    float pointSize = 1.0f;
    bool isWireframe = false;
};

struct TransformState {
    LA::mat4 projection = LA::mat4();
    LA::mat4 view = LA::mat4();
    std::stack<LA::mat4> transforms = std::stack<LA::mat4>({LA::mat4()});
    
};

// forward declarations

class Renderer : public Module {
protected:
    RendererState _state = RendererState();
    RenderStats _stats = RenderStats();
    TransformState _transforms = TransformState();

    Renderer(const std::string& name);
    
public:
    virtual ~Renderer() = default;

    static Renderer& Instance();

    // module interface
    virtual bool Boot() = 0;
    virtual bool Shutdown() = 0;
    
    // validation
    virtual bool ValidateShader(std::shared_ptr<Shader> shader, std::string& err_msg) = 0;
    virtual bool ValidateMesh(std::shared_ptr<Mesh> mesh, std::string& err_msg) = 0;

    /// --- Drawing ---
    // clear active canvas/screen of all color, depth, and stencil buffers
    virtual void Clear() = 0;
    // clear active canvas/screen according to bools args
    virtual void Clear(bool clearColor, bool clearStencil, bool clearDepth) = 0;

    // // Draw2D
    // virtual void Draw2DCircle() = 0;
    // virtual void Draw2DRectangle() = 0;

    // // Draw3D
    virtual void Draw(std::shared_ptr<Mesh> mesh) = 0;
    // virtual void DrawCube() = 0;

    /// --- State Management ---
    virtual void SetState(RendererState state) = 0;
    virtual void ResetState() = 0;
    virtual bool IsUsable() = 0;
    // colour
    virtual LA::vec4 GetClearColor() = 0;
    virtual LA::vec4 GetColorMask() = 0;
    virtual void SetClearColor(const LA::vec4& colour) = 0;
    virtual void SetColorMask(const LA::vec4& mask) = 0;
    // culling
    virtual bool GetCullTest() = 0;
    virtual CullFace GetCullFace() = 0;
    virtual CullWinding GetCullWinding() = 0;
    virtual void SetCullTest(bool enabled) = 0;
    virtual void SetCullFace(CullFace face) = 0;
    virtual void SetCullWinding(CullWinding winding) = 0;
    // depth triangle vertices we're defining them in a certain winding order that is either clockwise or counter-clockwise. Each triangle consists of 3 vertices and we specify those 3 vertices in a winding order as seen from the center of the triangle.
    virtual bool GetDepthTest() = 0;
    virtual DepthFunc GetDepthFunction() = 0;
    virtual bool GetDepthMask() = 0;
    virtual void SetDepthTest(bool enabled) = 0;
    virtual void SetDepthFunction(DepthFunc func) = 0;
    virtual void SetDepthMask(bool enabled) = 0;
    // rasterization
    virtual float GetLineWidth() = 0;
    virtual float GetPointSize() = 0;
    virtual bool GetIsWireframe() = 0;
    virtual void SetLineWidth(float width) = 0;
    virtual void SetPointSize(float size) = 0;
    virtual void SetIsWireframe(bool enabled) = 0;
    // bound objects
    // virtual std::shared_ptr<Canvas> GetCanvas() = 0;
    virtual std::shared_ptr<Shader> GetShader() = 0;
    // virtual void SetCanvas(std::shared_ptr<Canvas> canvas) = 0;
    virtual void SetShader(std::shared_ptr<Shader> shader) = 0;

    /// --- Coordinate System Transformations ---
    // camera and screen transformations
    virtual LA::mat4 GetProjection();
    virtual LA::mat4 GetView();
    virtual LA::mat4 GetModel();
    virtual void SetProjection(const LA::mat4& proj);
    virtual void SetView(const LA::mat4& view);
    // coordinate space transformations
    virtual LA::mat4 PopTransform();
    virtual void PushTransform(const LA::mat4& transform);
    virtual void PushTranslate(float x, float y, float z);
    virtual void PushTranslate(LA::vec3 translate);
    virtual void PushRotate(float x, float y, float z);
    virtual void PushRotate(LA::vec3 rotate);
    virtual void PushScale(float x, float y, float z);
    virtual void PushScale(LA::vec3 scale);
    // virtual LA::vec3 ScreenToGlobal(const LA::vec2& point);
    // virtual LA::vec2 GlobalToScreen(const LA::vec3& point);

    /// --- Shader Methods ---
    virtual bool HasUniform(const std::string& key) = 0;

    // single value uniforms
    virtual bool SetUniform(const std::string& key, int value) = 0;
    virtual bool SetUniform(const std::string& key, uint32_t value) = 0;
    virtual bool SetUniform(const std::string& key, float value) = 0;
    virtual bool SetUniform(const std::string& key, double value) = 0;
    // vector uniforms
    virtual bool SetUniform(const std::string& key, const LA::vec2& v) = 0;
    virtual bool SetUniform(const std::string& key, float x, float y) = 0;
    virtual bool SetUniform(const std::string& key, const LA::vec3& v) = 0;
    virtual bool SetUniform(const std::string& key, float x, float y, float z) = 0;
    virtual bool SetUniform(const std::string& key, const LA::vec4& v) = 0;
    virtual bool SetUniform(const std::string& key, float x, float y, float z, float w) = 0;
    // matrix uniforms
    virtual bool SetUniform(const std::string& key, const LA::mat2& m) = 0;
    virtual bool SetUniform(const std::string& key, const LA::mat3& m) = 0;
    virtual bool SetUniform(const std::string& key, const LA::mat4& m) = 0;
    // uniform property
    virtual bool SetUniform(const std::string& key, const UniformProperty& value) = 0;
    
    /// --- Debug Info ---
    virtual RenderStats GetRenderStats();
    virtual void NextFrame();

};

} // renderer

} // marathon
