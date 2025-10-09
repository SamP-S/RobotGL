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
#include <ArduinoLog.h>
#include <Servo.h>
#include <AccelStepper.h>
#include <Arduino.h>

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

// ====================
// = AXIS
// ====================
class IAxis {
public:
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void set(float target) = 0;
    virtual float get() = 0;
    virtual void poll() = 0;
};

#define STEPPER_MAX_SPEED 1000
#define STEPPER_ACCELERATION 500
#define STEPPER_STEPS_PER_REV 200   // 1.8 degrees per step - no microstepping
#define STEPPER_STEP_ANGLE (360.0f / STEPPER_STEPS_PER_REV)
#define STEPPER_INTERFACE AccelStepper::DRIVER

class StepperAxis : public IAxis {
private:

    // motor control
    int _stepPin;
    int _dirPin;
    AccelStepper _stepper; 
    // user defined constraints
    int _minAngle;
    int _maxAngle;
    float _gearRatio;
    // motor constraints
    const int _MIN_MIN_ANGLE = 0;
    const int _MAX_MAX_ANGLE = 180;
    const int _ANGLE_OFFSET = 90;

public:
    StepperAxis(int stepPin, int dirPin, int enPin, int minAngle, int maxAngle, float gearRatio = 1.0f)
        : _stepPin(stepPin), _dirPin(dirPin), _minAngle(minAngle), _maxAngle(maxAngle), _gearRatio(gearRatio),
            _stepper(STEPPER_INTERFACE, stepPin, dirPin) {
        _stepper.setMaxSpeed(STEPPER_MAX_SPEED);
        _stepper.setAcceleration(STEPPER_ACCELERATION);
    }

    void enable() override {
        digitalWrite(enPin, LOW);
    }

    void disable() override {
        digitalWrite(enPin, HIGH);
    }

    void set(float target) override {
        float targetAngle = constrain(target, _minAngle, _maxAngle);
        int targetSteps = targetAngle / STEPPER_STEP_ANGLE;
        _stepper.moveTo(targetSteps);
    }
    
    float get() override {
        return _stepper.currentPosition() * STEPPER_STEP_ANGLE;
    }

    void poll() {
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
    DualStepperAxis(int stepAPin, int dirAPin, int enAPin, int stepBPin, int dirBPin, int enBPin, float minAngle, float maxAngle, float gearRatio = 1.0f)
        : _stepAPin(stepAPin), _dirAPin(dirAPin), _stepBPin(stepBPin), _dirBPin(dirBPin), _minAngle(minAngle), _maxAngle(maxAngle), _gearRatio(gearRatio),
          _stepperA(STEPPER_INTERFACE, stepAPin, dirAPin), _stepperB(STEPPER_INTERFACE, stepBPin, dirBPin) {
        _stepperA.setMaxSpeed(STEPPER_MAX_SPEED);
        _stepperA.setAcceleration(STEPPER_ACCELERATION);
        _stepperB.setMaxSpeed(STEPPER_MAX_SPEED);
        _stepperB.setAcceleration(STEPPER_ACCELERATION);
    }

    void enable() override {
        digitalWrite(enAPin, LOW);
        digitalWrite(enBPin, LOW);
    }

    void disable() override {
        digitalWrite(enAPin, HIGH);
        digitalWrite(enBPin, HIGH);
    }

    void set(float target) override {
        float targetAngle = constrain(target, _minAngle, _maxAngle);
        int targetSteps = targetAngle / STEPPER_STEP_ANGLE;
        _stepperA.moveTo(targetSteps);
        _stepperB.moveTo(-targetSteps);
    }
    
    float get() override {
        return _stepperA.currentPosition() * STEPPER_STEP_ANGLE;
    }

    void poll() {
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
        : _pin(pin), _minAngle(minAngle), _maxAngle(maxAngle), _gearRatio(gearRatio),
          _servo(Servo()) {}

    void enable() override {
        _servo.attach(_pin);
    }   

    void disable() override {
        _servo.detach();
    }

    void set(float target) override {
        float targetAngle = constrain(target, _minAngle, _maxAngle);
        _servo.write(targetAngle);
    }

    float get() override {
        return _servo.read();
    }
};

class Robot {
public:
    Robot() {

    }

    // set
    void enable();
    void disable();

    // pose control
    void setPose(float* angles, int axisCount);
    void getPose(float* angles, int axisCount);
    void setAxis(int axisID, float angle);
    void getAxis(int axisID, float* angle);

    // predefined movement
    void home();

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
    

};


// ====================
// = COMMANDS OPERATIONS
// ====================

IAxis* axes = nullptr;
int axisCount = 0;

void enable(String* args, int argCount) {
    if (argCount != 1) {
        Serial.println("ENABLE requires 0 arguments.");
        return;
    }
    for (int i = 0; i < axisCount; i++) {
        axes[i].enable();
    }
}

void disable(String* args, int argCount) {
    if (argCount != 1) {
        Serial.println("DISABLE requries 0 arguments.");
        return;
    }
    for (int i = 0; i < axisCount; i++) {
        axes[i].disable();
    }
}

void set(String* args, int argCount) {
    if (argCount != axisCount + 1) {
        Serial.print("SET requires "); 
        Serial.print(AXIS_COUNT); 
        Serial.println(" arguments: <angle>..."); 
        return;
    }
    for (int i = 0; i < axisCount; i++) {
        float angle = args[1+i].toFloat();
        axes[i].set(angle);
        Serial.print("Axis ");
        Serial.print(i);
        Serial.print(" set to angle ");
        Serial.println(angle);
    }
}

void get(String* args, int argCount) {
    if (argCount != 1) {
        Serial.print("GET requires 0 arguments.");
        return;
    }
    for (int i = 0; i < axisCount; i++) {
        Serial.print("Axis ");
        Serial.print(axes[i].get());
        Serial.print(" set to angle ");
        Serial.println(angle);
    }
}

void setAxis(String* args, int argCount) {
    if (argCount != 3) {
        Serial.println("SET_AXIS requires 2 arguments: <axis> <angle>");
        return;
    }
    int axis = args[1].toInt();
    float angle = args[2].toFloat();
    if (axis < 0 || axis >= axisCount) {
        Serial.println("Invalid axis index");
        return;
    }
    axes[axis].set(angle);
    Serial.print("Axis ");
    Serial.print(axis);
    Serial.print(" set to angle ");
    Serial.println(angle);
}

void getAxis(String* args, int argCount) {
    if (argCount != 2) {
        Serial.println("ERROR: GET_AXIS requires 1 argument: <axis>");
        return;
    }
    int axis = args[1].toInt();
    if (axis < 0 || axis >= axisCount) {
        Serial.println("ERROR: Invalid axis index");
        return;
    }
    float angle = axes[axis].get();
    Serial.print("Axis ");
    Serial.print(axis);
    Serial.print(" angle: ");
    Serial.println(angle);
}

void home(String* args, int argCount) {
    for (int i = 0; i < axisCount; i++)
        axes[i].set(0.0f);
}

// ====================
// = SERIAL COMMANDS
// ====================
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
    enable, // CMD_ENABLE
    disable, // CMD_DISABLE
    set, // CMD_SET
    get, // CMD_GET
    setAxis, // CMD_SET_AXIS
    getAxis, // CMD_GET_AXIS
    nullptr, // CMD_STOP
    nullptr, // CMD_PAUSE
    nullptr, // CMD_RESUME
    home, // CMD_HOME
    nullptr, // CMD_RESET
    nullptr, // CMD_STATUS
};

void processCommand(const String& cmd) {
    // parse command
    Serial.println("INFO: Received command: " + cmd);
    String* args = new String[12];
    int argCount = 0;
    parseCommand(cmd, args, argCount);

    // empty command
    if (argCount == 0) {
        Serial.println("WARNING: Empty command");
        return;
    }

    // map command to enum
    int cmdID = args[0].toInt();
    if (cmdID < 0 || cmdID >= COMMAND::CMD_MAX_ENUM) {
        Serial.println("WARNING: Invalid command index");
        return;
    } else if (cmdID == CMD_UNKNOWN) {
        Serial.println("WARNING: Unknown command");
        return;
    }

    // execute command
    if (funcs[cmdID] != nullptr) {
        funcs[cmdID](args, argCount);
    } else {
        Serial.println("WARNING: Command not implemented");
    }

    // clean up
    delete[] args;
}

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
    
    // declare stepper motor test
    axisCount = 1;
    axes = new StepperAxis[1]{
        StepperAxis(stepXPin, dirXPin, enPin, 0.0f, 180.0f)
    };

    // // declare stepper motor robot section
    // axisCount = 3;
    // axes = new StepperAxis[3]{
    //     DualStepperAxis(stepXPin, dirXPin, enPin, stepYPin, dirYPin, enPin, 0.0f, 180.0f),
    //     StepperAxis(stepZPin, dirZPin, enPin, 0.0f, 180.0f),
    //     StepperAxis(stepAPin, dirAPin, enPin, 0.0f, 180.0f)
    // };
}

void loop() {
    String inputBuffer = "";
    bool cmdReading = false;
    bool cmdFinished = false;

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
            axes[i].poll();
        }
    }

    // allow serial to catch up
    delay(50);
}