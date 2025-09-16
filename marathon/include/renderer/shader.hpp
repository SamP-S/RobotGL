#pragma once

// PUBLIC HEADER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "la_extended.h"
#include "core/resource.hpp"

namespace marathon {

namespace renderer {


/// TODO:
// add support for seperated shader stages utilising defaults
// support a general default data layout for vertex attributes
// support a wider range of uniform sends

// enum class ShaderType {
//     VERTEX,
//     FRAGMENT
// };

/// TODO:
// shaders should determine what vertex attributes and uniforms are available
// shaders should use a standard layout
// custom mesh layouts should require custom shaders
// meshes (vertex attributes) must find the shader that matches their layout
// materials (uniforms) must match the shader's uniforms
// NOTE: unity uses multiple shader variances per possible missing vertex attribute and checks at runtime which to use according to mesh data available

enum class ShaderDirty {
    INVALID,
    DIRTY_COMPILE,
    DIRTY_DELETE,
    CLEAN
};

class Shader : public Resource {
protected:
    std::string _vSrc = "";
    std::string _fSrc = "";
    ShaderDirty _dirty = ShaderDirty::INVALID;
 
public:
    Shader();
    ~Shader();

    // empty everything, delete internal resources
    void Clear();

    const std::string& GetVertexSource() const;
    const std::string& GetFragmentSource() const;
    ShaderDirty GetDirtyFlag() const;

    // call to stop data being uploaded to GPU next frame
    void ClearDirtyFlag();
    // set shader sources
    void SetVertexSource(const std::string& vSrc);
    void SetFragmentSource(const std::string& fSrc);
    void SetSources(const std::string& vSrc, const std::string& fSrc);

};

}

}