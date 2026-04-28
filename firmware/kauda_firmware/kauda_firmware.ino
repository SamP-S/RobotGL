/*
    kauda_firmware.ino
    Description: Arduino firmware for Kauda Robot Arm.
    Implement axes control and communication over serial using commands
    Axes:
    1. Base Rotation
    2. Shoulder
    3. Elbow
    4. Wrist Rotation
    5. Wrist Bend
    6. End Effector (Gripper)

    Commands:
    - SET <angle> <angle> <angle> <angle> <angle> <angle>: Set all axes to specified angles.
    - GET: Get the current angles of all axes.
    - SET_AXIS <axis> <angle>: Set the specified axis to the given angle.
    - GET_AXIS <axis>: Get the current angle of the specified axis.
    - SET_FREE_MOVE: Set robot to free movement mode.
    - GET_FREE_MOVE: Get the free movement status.
    - STATUS: Get the current status of all axes.
    - HOME: Move all axes to their home position.
    - STOP: Stop all movements immediately.
*/

// std libs
#include <Servo.h>
#include <AccelStepper.h>
#include <Arduino.h>

// internal
#include "kauda_robot.h"

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

enum COMMAND : int {
    CMD_UNKNOWN = -1,
    // robot
    CMD_ENABLE,
    CMD_DISABLE,
    CMD_SET,
    CMD_GET,
    // motor
    CMD_SET_AXIS,
    CMD_GET_AXIS,
    // movement
    CMD_STOP,
    CMD_PAUSE,
    CMD_RESUME,
    // operation
    CMD_HOME,
    CMD_RESET,
    CMD_STATUS,
    CMD_MAX_ENUM,
};

class IMotor {
public:
    // angle
    virtual void Set(float value) = 0;
    virtual float Get() = 0;
    // operations
    virtual void Home() = 0;
    virtual void Reset() = 0;
    virtual void ReadStatus(char* str) = 0;
};

class ServoMotor : public IMotor {
private:
    Servo _servo;
    int _pin;
    float _angle;
    float _homeAngle;

public:
    ServoMotor(int pin, float homeAngle = 90.0f) : _pin(pin), _angle(homeAngle), _homeAngle(homeAngle) {
        _servo.attach(_pin);
        _servo.write(_angle);
    }

    // angle
    void Set(float value) override {
        _angle = constrain(value, 0.0f, 180.0f);
        _servo.write(_angle);
    }

    float Get() override {
        return _angle;
    }

    // operations
    void Home() override {
        Set(_homeAngle);
    }

    void Reset() override {
        Home();
    }

    void ReadStatus(char* str) override {
        sprintf(str, "Servo on pin %d angle: %.2f", _pin, _angle);
    }
};

class StepperMotor : public IMotor {
private:
    AccelStepper _stepper;
    int _stepPin, _dirPin;
    float _angle;
    float _homeAngle;
    float _stepsPerDegree;

public:
    StepperMotor(int stepPin, int dirPin, float homeAngle = 0.0f, float stepsPerDegree = 1.0f)
        : _stepPin(stepPin), _dirPin(dirPin), _angle(homeAngle), _homeAngle(homeAngle), _stepsPerDegree(stepsPerDegree),
            _stepper(AccelStepper::DRIVER, stepPin, dirPin) {
        _stepper.setMaxSpeed(1000);
        _stepper.setAcceleration(500);
        Set(_homeAngle);
    }

    // angle
    void Set(float value) override {
        float targetAngle = constrain(value, 0.0f, 180.0f);
        long targetSteps = targetAngle * _stepsPerDegree;
        _stepper.moveTo(targetSteps);
        while (_stepper.distanceToGo() != 0) {
            _stepper.run();
        }
        _angle = targetAngle;
    }

    float Get() override {
        return _angle;
    }

    // operations
    void Home() override {
        Set(_homeAngle);
    }

    void Reset() override {
        Home();
    }

    void ReadStatus(char* str) override {
        sprintf(str, "Stepper on pins %d/%d angle: %.2f", _stepPin, _dirPin, _angle);
    }
};


void setup() {
    Serial.begin(115200);

    // === Stepper Motor Pins (Outputs) ===
    pinMode(enPin, OUTPUT);         // Enable pin
    pinMode(stepXPin, OUTPUT);      // X Axis Step
    pinMode(dirXPin, OUTPUT);       // X Axis Direction
    pinMode(stepYPin, OUTPUT);      // Y Axis Step
    pinMode(dirYPin, OUTPUT);       // Y Axis Direction
    pinMode(stepZPin, OUTPUT);      // Z Axis Step
    pinMode(dirZPin, OUTPUT);       // Z Axis Direction

    // === Limit Switch Pins (Inputs with Pullups) ===
    pinMode(limitSwitchXPin, INPUT_PULLUP);  // X Limit Switch
    pinMode(limitSwitchYPin, INPUT_PULLUP);  // Y Limit Switch
    pinMode(limitSwitchZPin, INPUT_PULLUP);  // Z Limit Switch

    // === Motion Control Buttons (Inputs with Pullups) ===
    pinMode(abortPin, INPUT_PULLUP);   // Abort / Emergency Stop
    pinMode(holdPin, INPUT_PULLUP);    // Feed Hold / Pause
    pinMode(resumePin, INPUT_PULLUP);  // Resume / Cycle Start

    // === Coolant & Spindle Outputs ===
    pinMode(coolEnPin, OUTPUT);     // Coolant Enable
    pinMode(spinEnPin, OUTPUT);     // Spindle Enable
    pinMode(spinDirPin, OUTPUT);    // Spindle Direction
}

void loop() {
    while (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        processCommand(cmd);
    }

    // allow serial to catch up
    delay(10);
}