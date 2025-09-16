#include "renderer/material.hpp"

#include <iostream>
#include "core/logger.hpp"
#include "renderer/renderer.hpp"

namespace marathon {

namespace renderer {

//// Material -------------------------------------------------------------------

Material::Material()
    : Resource("marathon.renderer.material") {
    _mShader = std::make_shared<Shader>();
}

Material::~Material() {}

std::shared_ptr<Shader> Material::GetShader() const {
    return _mShader;
}

void Material::SetShader(std::shared_ptr<Shader> shader) {
    _mShader = shader;
}

bool Material::HasUniform(const std::string& key) const {
    return _mUniforms.find(key) != _mUniforms.end();
}

const std::unordered_map<std::string, UniformProperty>& Material::GetUniforms() const {
    return _mUniforms;
}

UniformProperty Material::GetUniform(const std::string& key) const {
    auto it = _mUniforms.find(key);
    if (it == _mUniforms.end()) {
        MT_CORE_ERROR("Material::GetUniform(): uniform {} not found in material uniforms", key);
        return UniformProperty(); // return default constructed UniformProperty
    }
    return it->second;
}

void Material::SetUniform(const std::string& key, UniformProperty value) {
    _mUniforms[key] = value;
}


//// ColourMaterial ------------------------------------------------------------

std::string ColourMaterial::_sVertexSource = R"(
void main()
{
    varying_position = vec4(vertex_position, 1.0f);
	gl_Position =  u_projection * u_view * u_model * vec4(vertex_position, 1.0f);
}
)";
std::string ColourMaterial::_sFragmentSource = R"(
// outputs
layout(location = 0) out vec4 out_color;

uniform vec4 u_colour;

void main()
{
    out_color = u_colour;
}
)";

ColourMaterial::ColourMaterial() {
    // create shader
    _mShader = std::make_shared<Shader>();
    _mShader->SetSources(_sVertexSource, _sFragmentSource);

    _mUniforms["u_colour"] = LA::vec4({1.0f, 1.0f, 1.0f, 1.0f});
};
ColourMaterial::~ColourMaterial() {};

LA::vec4 ColourMaterial::GetColour() const {
    auto it = _mUniforms.find("u_colour");
    // check if uniform key in map
    if (it == _mUniforms.end()) {
        MT_CORE_ERROR("ColourMaterial::GetColour(): uniform \"u_colour\" not found in material uniforms");
        return LA::vec4(1.0f);
    }
    // check uniform type is correct
    if (!std::holds_alternative<LA::vec4>(it->second)) {
        MT_CORE_ERROR("ColourMaterial::GetColour(): uniform \"u_colour\" variant does not contain correct type");
        return LA::vec4(1.0f);
    }
    return std::get<LA::vec4>(it->second);

}
void ColourMaterial::SetColour(LA::vec4 colour) {
    auto it = _mUniforms.find("u_colour");
    // check if uniform key in map
    if (it == _mUniforms.end()) {
        MT_CORE_WARN("ColourMaterial::SetColour(): uniform \"u_colour\" not found in material uniforms");
    }
    // assign/reassign/create either way
    _mUniforms["u_colour"] = colour;
}

} // renderer

} // marathon
