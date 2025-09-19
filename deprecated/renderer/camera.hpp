#pragma once

#include "core/node.hpp"
#include "la_extended.h"

namespace marathon {

namespace renderer {

/// TODO: how to provide getters/setters to specific camera properties
/// e.g. to allow fov to be changed without storing config values externally

class Camera : public Node {
public:
    enum class Projection {
        NONE,
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

private:
    Projection _mProjection = Projection::NONE;    

    // orthoganal configuration
    float _mLeft = -1.0f;
    float _mRight = 1.0f;
    float _mBottom = -1.0f;
    float _mTop = 1.0f;
    float _mNearOrtho = 0.1f;
    float _mFarOrtho = 100.0f;

    // projection configuration
    float _mFov = 45.0f;
    float _mNear = 0.1f;
    float _mFar = 100.0f;
    /// aspect ratio is calculated from the window size

    LA::mat4 _mProjectionMatrix = LA::mat4(1.0f);
    std::weak_ptr<Camera> _mActiveCamera = {};

public:
    void SetPerspective(float fov, float aspect, float near, float far);
    void SetOrthographic(float left, float right, float bottom, float top, float near, float far);
    void ClearProjection();

    Projection GetProjection() const;
    LA::mat4 GetProjectionMatrix() const;

    // can only be one active camera at a time
    bool IsActive() const;
    void SetActive();

};

} // namespace renderer

} // namespace marathon