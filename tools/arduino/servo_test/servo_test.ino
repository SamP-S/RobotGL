#include <Servo.h>

Servo myServo;
const int servoPin = 9;
const int minAngle = 60;
const int maxAngle = 120;
const int delayTime = 15; // ms

void setup() {
    myServo.attach(servoPin);
}

void loop() {
    // myServo.write(minAngle);
    // delay(1000);
    // myServo.write(maxAngle);
    // delay(1000);

    myServo.write(0);
    delay(1000);
    myServo.write(90);
    delay(1000);
    myServo.write(180);
    delay(1000);
}