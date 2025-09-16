#include "renderer/renderer.hpp"
#include "renderer/opengl/renderer.hpp"

namespace marathon {

namespace renderer {

Renderer::Renderer(const std::string& name)
    : marathon::Module(ModuleType::RENDERER, name) {}

Renderer& Renderer::Instance() {
    static Renderer* instance;
    if (!instance)
        instance = new marathon::renderer::opengl::Renderer();
    return *instance;
}


LA::mat4 Renderer::GetProjection() {
    return _transforms.projection;
}
LA::mat4 Renderer::GetView() {
    return _transforms.view;
}
LA::mat4 Renderer::GetModel() {
    return _transforms.transforms.top();
}
void Renderer::SetProjection(const LA::mat4& proj) {
    _transforms.projection = proj;
}
void Renderer::SetView(const LA::mat4& view) {
    _transforms.view = view;
}

LA::mat4 Renderer::PopTransform() {
    if (_transforms.transforms.size() <= 1)
        return LA::mat4();
    LA::mat4 top = _transforms.transforms.top();
    _transforms.transforms.pop();
    return top;
}
void Renderer::PushTransform(const LA::mat4& transform) {
    LA::mat4 cur = _transforms.transforms.top();
    LA::mat4 next = cur * transform;
    _transforms.transforms.push(next);
}
void Renderer::PushTranslate(float x, float y, float z) {
    PushTransform(LA::Translate(x, y, z));
}
void Renderer::PushTranslate(LA::vec3 translate) {
    PushTransform(LA::Translate(translate));
}
void Renderer::PushRotate(float x, float y, float z) {
    PushTransform(LA::Rotate(x, y, z));
}
void Renderer::PushRotate(LA::vec3 rotate) {
    PushTransform(LA::Rotate(rotate));
}
void Renderer::PushScale(float x, float y, float z) {
    PushTransform(LA::Scale(x, y, z));
}
void Renderer::PushScale(LA::vec3 scale) {
    PushTransform(LA::Scale(scale));
}

RenderStats Renderer::GetRenderStats() {
    return _stats;
}

void Renderer::NextFrame() {
    _stats.frameIndex++;
    _stats.drawCalls = 0;
    _stats.trianglesRendered = 0;
}

} // renderer

} // marathon