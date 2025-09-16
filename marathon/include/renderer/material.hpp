#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <variant>

#include "la_extended.h"
#include "core/resource.hpp"
#include "renderer/shader.hpp"

namespace marathon {

namespace renderer {

typedef std::variant<int, uint32_t, float, double, LA::vec2, LA::vec3, LA::vec4, LA::mat2, LA::mat3, LA::mat4> UniformProperty;

/// TODO: implement automatic uniforms setup from shader

class Material : public Resource {
protected:
    std::shared_ptr<renderer::Shader> _mShader;
    std::unordered_map<std::string, UniformProperty> _mUniforms;

public:
    Material();
    ~Material();

    std::shared_ptr<renderer::Shader> GetShader() const;
    void SetShader(std::shared_ptr<renderer::Shader> shader);

    bool HasUniform(const std::string& key) const;
    const std::unordered_map<std::string, UniformProperty>& GetUniforms() const;
    UniformProperty GetUniform(const std::string& key) const;
    void SetUniform(const std::string& key, UniformProperty value);
};


class ColourMaterial : public Material {
private:
    static std::string _sVertexSource;
    static std::string _sFragmentSource;

public:
    ColourMaterial();
    ~ColourMaterial();

    LA::vec4 GetColour() const;
    void SetColour(LA::vec4 colour);
};

} // renderer

} // marathon
