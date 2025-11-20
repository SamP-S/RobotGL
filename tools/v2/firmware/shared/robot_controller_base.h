#pragma once

#include "serial_interface.h"
#include "pose.h"

class RobotControllerBase {
protected:
    SerialInterface m_iface;
    Pose m_poseBuffer[POSE_BUFFER_SIZE];

    uint8_t m_activePose = 0;
    bool m_moving = false;

    void handleSetPose(id_t _id, const float _angles[POSE_ANGLE_COUNT]) {
        if (_id == 0) {
            m_iface.sendRejected(_id, "ID_ZERO_IS_RESERVED");
            return;
        } else if (_id >= POSE_BUFFER_SIZE) {
            m_iface.sendRejected(_id, "ID_OUT_OF_RANGE");
            return;
        }
        for (int i = 0; i < POSE_ANGLE_COUNT; ++i) {
            m_poseBuffer[_id].angles[i] = _angles[i];
        }
        m_iface.sendAccepted(_id);
    }

    void handleGetPose(id_t _id) {
        if (_id >= POSE_BUFFER_SIZE) {
            m_iface.sendRejected(_id, "ID_OUT_OF_RANGE");
            return;
        }
        m_iface.sendReturnPose(_id, m_poseBuffer[_id]);
    }

    virtual void handleGoTo() = 0;
    virtual void handleStop() = 0;

public:
    RobotControllerBase(SerialInterface& _si)
        : m_serial(_si) {
        // initialize pose buffer to zero
        for (int i = 0; i < POSE_BUFFER_SIZE; ++i) {
            for (int j = 0; j < POSE_ANGLE_COUNT; ++j) {
                m_poseBuffer[i].angles[j] = 0.0f;
            }
        }
    }

    virtual void setup() = 0;
    virtual void loop() = 0;

};
