/*
    kauda_firmware.ino
    Description: Arduino firmware for Kauda Robot Arm.
    Implement axes control and communication over serial.
    Axes:
    1. Base Rotation
    2. Shoulder
    3. Elbow
    4. Wrist Rotation
    5. Wrist Bend
    6. End Effector (Gripper)
*/

// Initialize serial communication at 9600 baud
void setup() {    
    Serial.begin(115200);
}

// Main code runs repeatedly
void loop() {
    Serial.println("Hello from kauda.ino!");
    delay(1000);
}