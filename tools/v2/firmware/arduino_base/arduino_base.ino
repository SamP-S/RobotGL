#include <Arduino.h>
#include <AccelStepper.h>
#include "pose.h"
#include "serial_interface.h"
#include "robot_controller_base.h"

#define STEPS_PER_REV 200
#define DEGREES_PER_STEP (360.0f / STEPS_PER_REV)  // 1.8 degrees per step
#define STEPS_PER_DEG (1.0f / DEGREES_PER_STEP)

// motor pins
const int enPin = 8;
const int stepXPin = 2;     // X.STEP
const int dirXPin = 5;      // X.DIR
const int stepYPin = 3;     // Y.STEP
const int dirYPin = 6;      // Y.DIR
const int stepZPin = 4;     // Z.STEP
const int dirZPin = 7;      // Z.DIR

// limit switch pins
const int limitSwitchXPin = 9; // X.MIN
const int limitSwitchYPin = 10; // Y.MIN
const int limitSwitchZPin = 11; // Z.MIN

// motion control
const int abortPin = A0;
const int holdPin = A1;
const int resumePin = A2;

// coolant/spindle
const int coolEnPin = A3;
const int spinEnPin = 12;
const int spinDirPin = 13;

// Gear ratio placeholders (all reductions -> >= 1.0)
const float  kGearJ1 = 63.0f / 15.0f;
const float  kGearJ2 = 55.0f / 9.0f;
const float  kGearJ3 = 44.5f / 9.0f;

// operational angle limits (deg)
const float  kJointLimitDeg = 90.0f;

// Robot controller for stepper motors (joints 1-3)
// Joint mapping:
//      angles[0] = J1 (base)
//      angles[1] = J2 (shoulder, two steppers reverse),
//      angles[2] = J3 (elbow)
// All joints are bound to within -90 to +90 degrees
// Stepper motors do not have microstepping so 200 steps per revolution (1.8 degrees per step)
// Assumed boot in home position (all joints at 0 degrees)
// Using arduino uno with v3 CNC controller shield

class StepperController : public RobotControllerBase {
private:
    AccelStepper m_stepperJ1;
    AccelStepper m_stepperJ2;
    // reverse is copying through shield, so controller not needed
    AccelStepper m_stepperJ3;

    id_t m_activePoseID = 0;
    id_t m_targetPoseID = 0;
    bool moving = false;

    // convert delta degrees to steps (signed)
    long angleDeltaToSteps(float _deltaDeg, float _gearRatio) {
        float steps = _deltaDeg * STEPS_PER_DEG * _gearRatio;
        return (long)round(steps);
    }

    void handleGoTo(id_t _id) override {
        // validate id
        if (_id >= POSE_BUFFER_SIZE) {
            m_iface.sendRejected(_id, "ID_OUT_OF_RANGE");
            return;
        }
        // compute constrained target angles for joints 0..2
        float activeAngles[3];
        float targetAngles[3];
        for (int i = 0; i < 3; ++i) {
            activeAngles[i] = constrain(m_poseBuffer[m_activePoseID].angles[i], -kJointLimitDeg, kJointLimitDeg);
            targetAngles[i] = constrain(m_poseBuffer[_id].angles[i], -kJointLimitDeg, kJointLimitDeg);
        }

        // compute delta degrees
        float deltaDegJ1 = targetAngles[0] - activeAngles[0];
        float deltaDegJ2 = targetAngles[1] - activeAngles[1];
        float deltaDegJ3 = targetAngles[2] - activeAngles[2];

        // convert to steps (signed)
        long stepsJ1 = angleDeltaToSteps(deltaDegJ1, kGearJ1);
        long stepsJ2 = angleDeltaToSteps(deltaDegJ2, kGearJ2);
        long stepsJ3 = angleDeltaToSteps(deltaDegJ3, kGearJ3);

        // prepare move: moveTo takes absolute target position, so add delta to currentPosition()
        long curJ1 = m_stepperJ1.currentPosition();
        long curJ2 = m_stepperJ2.currentPosition();
        long curJ3 = m_stepperJ3.currentPosition();

        // Shoulder uses two steppers in reverse: primary moves +steps, reverse moves -steps
        m_stepperJ1.moveTo(curJ1 + stepsJ1);
        m_stepperJ2.moveTo(curJ2 + stepsJ2);
        m_stepperJ3.moveTo(curJ3 + stepsJ3);

        // enable drivers (assume LOW = enabled; adjust if your hardware differs)
        digitalWrite(enPin, LOW);

        m_targetPoseID = _id;
        moving = true;
        m_iface.sendAccepted(_id);
    }

    void handleStop() override {
        // Stop motion as soon as possible; keep drivers enabled to hold position if possible
        m_stepperJ1.stop();
        m_stepperJ2.stop();
        m_stepperJ3.stop();
        moving = false;
        m_iface.sendStopped();
        // Note: to let motors go limp, set digitalWrite(enPin, HIGH) here instead.
    }

public:
    StepperController(SerialInterface& _iface)
        : RobotControllerBase(_iface),
            m_stepperJ1(AccelStepper::DRIVER, stepXPin, dirXPin),
            m_stepperJ2(AccelStepper::DRIVER, stepYPin, dirYPin),
            m_stepperJ3(AccelStepper::DRIVER, stepZPin, dirZPin) {}

    void setup() override {
        // configure enable pin
        pinMode(enPin, OUTPUT);
        digitalWrite(enPin, LOW); // enable drivers (adjust polarity if needed)

        // basic limits for steppers
        const float maxSpeed = 1000.0f;      // steps/sec
        const float maxAccel = 500.0f;       // steps/sec^2

        m_stepperJ1.setMaxSpeed(maxSpeed);
        m_stepperJ1.setAcceleration(maxAccel);
        m_stepperJ2.setMaxSpeed(maxSpeed);
        m_stepperJ2.setAcceleration(maxAccel);
        m_stepperJ3.setMaxSpeed(maxSpeed);
        m_stepperJ3.setAcceleration(maxAccel);

        // assume boot in home position => zero current position
        m_stepperJ1.setCurrentPosition(0);
        m_stepperJ2.setCurrentPosition(0);
        m_stepperJ3.setCurrentPosition(0);
    }

    void loop() override {
        ParsedCommand cmd;
        if (m_iface.poll(cmd)) {
            if (!cmd.valid) {
                return;    // invalid command already handled
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

        // if moving, step motors
        if (moving) {
            // call run() often to step the motors toward their targets
            m_stepperJ1.run();
            m_stepperJ2.run();
            m_stepperJ3.run();

            // check completion: all steppers have reached target
            if (m_stepperJ1.distanceToGo() == 0 &&
                m_stepperJ2.distanceToGo() == 0 &&
                m_stepperJ3.distanceToGo() == 0) {
                moving = false;
                // update active pose and notify
                m_activePoseID = m_targetPoseID;
                m_iface.sendCompleted(m_activePoseID);
            }
        }

        delay(5);  // small delay to avoid overwhelming Serial
    }
};

SerialInterface serialInterface(Serial);
StepperController controller(serialInterface);

void setup() {
    Serial.begin(115200);
    // wait for Serial Monitor
    while (!Serial);
    delay(50);
    // clear any existing input
    while (Serial.available()) {
        Serial.read();
    }
    controller.setup();
}

void loop() {
    controller.loop();
}
