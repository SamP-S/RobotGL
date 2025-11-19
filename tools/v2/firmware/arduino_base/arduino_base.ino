#include <Arduino.h>
#include "serial_interface.h"

SerialInterface si(Serial);
ParsedCommand cmd;

void setup() {
    Serial.begin(115200);
    while (!Serial);  // wait for Serial Monitor
}

void loop() {
    // Poll for any new command from serial
    if (si.poll(cmd)) {
        if (!cmd.valid) {
            return;
        }

        switch (cmd.type) {
            case CMD_SET_POSE:
                break;

            case CMD_GET_POSE:
                break;

            case CMD_GO_TO:
                break;

            case CMD_STOP:
                break;

            default:
                break;
        }
        
    }

}
