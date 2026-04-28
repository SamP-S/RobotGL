#include <Arduino.h>
#include "serial_interface.h"

SerialInterface si(Serial);
ParsedCommand cmd;

void setup() {
    Serial.begin(115200);
    while (!Serial);  // wait for Serial Monitor

    Serial.println(F("SerialInterface test started"));
}

void loop() {
    // Poll for any new command from serial
    if (si.poll(cmd)) {
        // Print parsed command for debugging
        Serial.print(F("Received command: "));
        switch (cmd.type) {
            case CMD_SET_POSE:
                Serial.print(F("SET_POSE id="));
                Serial.print(cmd.id);
                Serial.print(F(" angles="));
                for (int i = 0; i < 5; i++) {
                    Serial.print(cmd.angles[i], 5);
                    if (i < 4) Serial.print(F(","));
                }
                Serial.println();
                break;

            case CMD_GET_POSE:
                Serial.print(F("GET_POSE id="));
                Serial.println(cmd.id);
                break;

            case CMD_GO_TO:
                Serial.print(F("GO_TO id="));
                Serial.println(cmd.id);
                break;

            case CMD_STOP:
                Serial.println(F("STOP command received"));
                break;

            default:
                Serial.println(F("Unknown command type"));
                break;
        }
        if (!cmd.valid) {
            Serial.println(F("Command was invalid, should receive error"));
        }
    }

    // Optional: echo back any raw input for debugging
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
            Serial.print(F("Raw input: "));
            Serial.println(line);
        }
    }

    delay(5);  // small delay to avoid overwhelming Serial
}
