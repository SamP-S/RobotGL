#include <Arduino.h>
#include <Servo.h>
#include "pose.h"
#include "serial_interface.h"
#include "robot_controller_base.h"

#define SERVO_MIN 0
#define SERVO_MAX 180
#define SERVO_MID 90
typedef unsigned long mtime_t;

class ServoController : public RobotControllerBase {
private:
    Servo m_j3Forearm;
    Servo m_j4Wrist;

    id_t m_targetPoseID = 0;

    void handleGoTo(id_t _id) override {
        if (_id >= POSE_BUFFER_SIZE) {
            m_iface.sendRejected(_id, "ID_OUT_OF_RANGE");
            return;
        }
        m_targetPoseID = _id;
        int j3Angle = transformAngleToServo(m_poseBuffer[_id].angles[3], -90.0f, 90.0f);
        int j4Angle = transformAngleToServo(m_poseBuffer[_id].angles[4], -90.0f, 90.0f);
        m_j3Forearm.write(j3Angle);
        m_j4Wrist.write(j4Angle);
        m_iface.sendAccepted(_id);
    }

    void handleStop() override {
        // cant stop servos??
        m_iface.sendStopped(m_targetPoseID);
    }

    int transformAngleToServo(float _angle, float _angleMin, float _angleMax) {
        int transformed = (int)(constrain(_angle, _angleMin, _angleMax) + 90.0f);
        return constrain(transformed, SERVO_MIN, SERVO_MAX);
    }

public:
    ServoController(SerialInterface& _si)
        : RobotControllerBase(_si) {}

    void setup() override {
        m_j3Forearm.attach(9); // attach forearm servo to pin 9
        m_j4Wrist.attach(10);  // attach wrist servo to pin 10
    }

    void loop() override {
        ParsedCommand cmd;
        if (m_iface.poll(cmd)) {
            if (!cmd.valid) {
                return;     // invalid command already handled
            }
            switch (cmd.type) {
                case CMD_SET_POSE:
                    this->handleSetPose(cmd.id, cmd.angles);
                    break;
                case CMD_GET_POSE:
                    this->handleGetPose(cmd.id);
                    break;
                case CMD_GO_TO:
                    this->handleGoTo(cmd.id);
                    break;  
                case CMD_STOP:
                    this->handleStop();
                    break;
                default:
                    break;
            }
        }
    }
};

SerialInterface serialInterface(Serial);
ServoController controller(serialInterface);

void setup() {
    Serial.begin(115200);
    controller.setup();
}

void loop() {
    controller.loop();
}
