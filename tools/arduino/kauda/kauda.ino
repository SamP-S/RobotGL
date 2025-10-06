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
    CMD_UNKNOWN = 0,
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

// motor interface
class IAxis {
public:
    virtual void attach() = 0;
    virtual void detach() = 0;
    virtual void Set(float target) = 0;
    virtual float Get() = 0;
    virtual void Poll() = 0;
};

#define STEPPER_MAX_SPEED 1000
#define STEPPER_ACCELERATION 500
#define STEPPER_STEPS_PER_REV 200   // 1.8 degrees per step - no microstepping
#define STEPPER_STEP_ANGLE (360.0f / STEPPER_STEPS_PER_REV)
#define STEPPER_INTERFACE AccelStepper::DRIVER

class StepperAxis : public IAxis {
private:
    int _stepPin;
    int _dirPin;
    float _minAngle;
    float _maxAngle;
    float _gearRatio;
    AccelStepper _stepper; 

public:
    StepperAxis(int stepPin, int dirPin, float minAngle, float maxAngle, float gearRatio = 1.0f)
        : _stepPin(stepPin), _dirPin(dirPin), _minAngle(minAngle), _maxAngle(maxAngle), _angle(0.0f),
            _stepper(STEPPER_INTERFACE, stepPin, dirPin) {
        _stepper.setMaxSpeed(STEPPER_MAX_SPEED);
        _stepper.setAcceleration(STEPPER_ACCELERATION);
        attach();
    }

    void attach() override {
        // No specific attach needed for stepper
    }

    void detach() override {
        // No specific detach needed for stepper
    }

    void Set(float target) override {
        float targetAngle = constrain(target, _minAngle, _maxAngle);
        int targetSteps = targetAngle / STEPPER_STEP_ANGLE;
        _stepper.moveTo(targetSteps);
    }
    
    float Get() override {
        return _stepper.currentPosition() * STEPPER_STEP_ANGLE;
    }

    void Poll() {
        if (_stepper.distanceToGo() != 0) {
            _stepper.run();
        }
    }
};

class DualStepperAxis : public IAxis {
private:
    int _stepAPin;
    int _dirAPin;
    int _stepBPin;
    int _dirBPin;
    float _minAngle;
    float _maxAngle;
    float _gearRatio;
    AccelStepper _stepperA;
    AccelStepper _stepperB;

public:
    DualStepperAxis(int stepAPin, int dirAPin, int stepBPin, int dirBPin, float minAngle, float maxAngle, float gearRatio = 1.0f)
        : _stepAPin(stepAPin), _dirAPin(dirAPin), _stepBPin(stepBPin), _dirBPin(dirBPin), _minAngle(minAngle), _maxAngle(maxAngle), _gearRatio(gearRatio),
          _stepperA(STEPPER_INTERFACE, stepAPin, dirAPin), _stepperB(STEPPER_INTERFACE, stepBPin, dirBPin) {
        _stepperA.setMaxSpeed(STEPPER_MAX_SPEED);
        _stepperA.setAcceleration(STEPPER_ACCELERATION);
        _stepperB.setMaxSpeed(STEPPER_MAX_SPEED);
        _stepperB.setAcceleration(STEPPER_ACCELERATION);
    }

    void attach() override {
        // No specific attach needed for stepper
    }

    void detach() override {
        // No specific detach needed for stepper
    }

    void Set(float target) override {
        float targetAngle = constrain(target, _minAngle, _maxAngle);
        int targetSteps = targetAngle / STEPPER_STEP_ANGLE;
        _stepperA.moveTo(targetSteps);
        _stepperB.moveTo(-targetSteps);
    }
    
    float Get() override {
        return _stepperA.currentPosition() * STEPPER_STEP_ANGLE;
    }

    void Poll() {
        if (_stepperA.distanceToGo() != 0) {
            _stepperA.run();
        }
        if (_stepperB.distanceToGo() != 0) {
            _stepperB.run();
        }
    }
};

class ServoAxis : public IAxis {
private:
    Servo _servo;
    int _pin;
    float _minAngle;
    float _maxAngle;
    float _gearRatio;

public:
    ServoAxis(int pin, float minAngle, float maxAngle, float gearRatio = 1.0f)
        : _pin(pin), _minAngle(minAngle), _maxAngle(maxAngle), _gearRatio(gearRatio) {
            _servo()
    }

    void attach() override {
        _servo.attach(_pin);
    }   

    void detach() override {
        _servo.detach();
    }

    void Set(float target) override {
        float targetAngle = constrain(target, _minAngle, _maxAngle);
        _servo.write(targetAngle);
    }

    float Get() override {
        return _servo.read();
    }
};

#define AXIS_COUNT 3
#define AXIS_HOME_ANGLE 0.0f

IAxis** axes = new IAxis*[AXIS_COUNT];
String inputBuffer = "";
bool cmdReading = false;
bool cmdFinished = false;

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

    digitalWrite(enPin, LOW); // Enable motors
    // init test motor
    axes[0] = new StepperAxis(stepXPin, dirXPin, 0.0f, 180.0f);
    // // init arm
    // axes[0] = new DualStepperAxis(stepXPin, dirXPin, stepYPin, dirYPin, 0.0f, 180.0f);
    // axes[1] = new StepperAxis(stepZPin, dirZPin, 0.0f, 180.0f);
    // axes[2] = new StepperAxis(stepAPin, dirAPin, 0.0f, 180.0f);
}

void setAxis(String* args, int argCount) {
    if (argCount < 3) {
        Serial.println("SET_AXIS requires 2 arguments: <axis> <angle>");
        return;
    }
    int axis = args[1].toInt();
    float angle = args[2].toFloat();
    if (axis < 0 || axis >= AXIS_COUNT) {
        Serial.println("Invalid axis index");
        return;
    }
    axes[axis]->Set(angle);
    Serial.print("Axis ");
    Serial.print(axis);
    Serial.print(" set to angle ");
    Serial.println(angle);
}

void getAxis(String* args, int argCount) {
    if (argCount < 2) {
        Serial.println("GET_AXIS requires 1 argument: <axis>");
        return;
    }
    int axis = args[1].toInt();
    if (axis < 0 || axis >= AXIS_COUNT) {
        Serial.println("Invalid axis index");
        return;
    }
    float angle = axes[axis]->Get();
    Serial.print("Axis ");
    Serial.print(axis);
    Serial.print(" angle: ");
    Serial.println(angle);
}

void home(String* args, int argCount) {
    for (int i = 0; i < AXIS_COUNT; i++)
        axes[i]->Set(AXIS_HOME_ANGLE);
}

void parseCommand(const String& cmd, String* args, int& argCount) {
    argCount = 0;
    int startIndex = 0;
    int spaceIndex;

    while ((spaceIndex = cmd.indexOf(' ', startIndex)) != -1) {
        args[argCount++] = cmd.substring(startIndex, spaceIndex);
        startIndex = spaceIndex + 1;
    }
    args[argCount++] = cmd.substring(startIndex);
}

void (*funcs[COMMAND::CMD_MAX_ENUM])(String* args, int argCount) = {
    nullptr, // CMD_UNKNOWN
    nullptr, // CMD_ENABLE
    nullptr, // CMD_DISABLE
    nullptr, // CMD_SET
    nullptr, // CMD_GET
    setAxis, // CMD_SET_AXIS
    getAxis, // CMD_GET_AXIS
    nullptr, // CMD_STOP
    nullptr, // CMD_PAUSE
    nullptr, // CMD_RESUME
    home, // CMD_HOME
    nullptr, // CMD_RESET
    nullptr, // CMD_STATUS
}

void processCommand(const String& cmd) {
    // parse command
    Serial.println("Received command: " + cmd);
    String* args = new String[8];
    int argCount = 0;
    parseCommand(cmd, args, argCount);

    // empty command
    if (argCount == 0) {
        Serial.println("Empty command");
        return;
    }

    // map command to enum
    int cmd = args[0].toInt();
    if (cmd < 0 || cmd >= COMMAND::CMD_MAX_ENUM) {
        Serial.println("Invalid command index");
        return;
    } else if (cmd == CMD_UNKNOWN) {
        Serial.println("Unknown command");
        return;
    }

    // execute command
    if (funcs[cmd] != nullptr) {
        funcs[cmd](args, argCount);
    } else {
        Serial.println("Command not implemented");
    }

    // clean up
    delete[] args;
}

void loop() {
    // read serial
    while (Serial.available() && !cmdFinished) {
        char c = Serial.read();
        if (!cmdReading && c == '<') {
            cmdReading = true;
            inputBuffer = "";
        } else if (!cmdReading) {
            continue; // Ignore until start of command
        } else if (cmdReading && c == '>') {
            cmdFinished = true;
            cmdReading = false;
        } else {
            inputBuffer += c;
        }
    }

    // exec cmd
    if (cmdFinished) {
        processCommand(inputBuffer);
        inputBuffer = "";
        cmdFinished = false;
    }

    // poll motors
    for (int i = 0; i < AXIS_COUNT; i++) {
        if (axes[i] != nullptr) {
            axes[i]->Poll();
        }
    }

    // allow serial to catch up
    delay(5);
}