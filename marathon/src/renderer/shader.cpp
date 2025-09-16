#include "renderer/shader.hpp"

namespace marathon {

namespace renderer {

Shader::Shader() 
    : Resource("marathon.renderer.shader") {}
Shader::~Shader() {}

void Shader::Clear() {
    _vSrc.clear();
    _fSrc.clear();
    _dirty = ShaderDirty::DIRTY_DELETE;
}

const std::string& Shader::GetVertexSource() const {
    return _vSrc;
}

const std::string& Shader::GetFragmentSource() const {
    return _fSrc;
}

ShaderDirty Shader::GetDirtyFlag() const {
    return _dirty;
}

void Shader::ClearDirtyFlag() {
    _dirty = ShaderDirty::CLEAN;
}

void Shader::SetVertexSource(const std::string& vSrc) {
    _vSrc = vSrc;
    _dirty = ShaderDirty::DIRTY_COMPILE;
}

void Shader::SetFragmentSource(const std::string& fSrc) {
    _fSrc = fSrc;
    _dirty = ShaderDirty::DIRTY_COMPILE;
}

void Shader::SetSources(const std::string& vSrc, const std::string& fSrc) {
    _vSrc = vSrc;
    _fSrc = fSrc;
    _dirty = ShaderDirty::DIRTY_COMPILE;
}

} // namespace renderer

} // namespace marathon