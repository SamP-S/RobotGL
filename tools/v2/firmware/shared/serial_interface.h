#pragma once

#include <Arduino.h>
#include "pose.h"

#define MAX_CMD_LENGTH 128
#define MAX_ID_LENGTH 16
#define MAX_ANGLE_LENGTH 64
typedef uint8_t id_t;

enum CommandType {
    CMD_UNKNOWN = 0,
    CMD_SET_POSE,
    CMD_GET_POSE,
    CMD_GO_TO,
    CMD_STOP,
    CMD_COUNT
};

struct ParsedCommand {
    CommandType type = CMD_UNKNOWN;
    id_t id = 0;
    float angles[POSE_ANGLE_COUNT] = {0};
    bool hasAngles = false;
    bool valid = false;
};

class SerialInterface {
private:
    Stream& m_serial;

    CommandType identifyType(const char* _cmd) {
    if (strncmp(_cmd, "SET_POSE", 8) == 0) {
            return CMD_SET_POSE;
        } else if (strncmp(_cmd, "GET_POSE", 8) == 0) {
            return CMD_GET_POSE;
        } else if (strncmp(_cmd, "GO_TO", 5) == 0) {
            return CMD_GO_TO;
        } else if (strncmp(_cmd, "STOP", 4) == 0) {
            return CMD_STOP;
        }
        return CMD_UNKNOWN;
    }

    // extract value of a key from a command string
    // returns true if successful, false if failed
    bool extractField(const char* _cmd, const char* _key, char* _out, size_t _outSize) {
        // check args given
        if (!_cmd || !_key || !_out) {
            this->sendError("EXTRACT_INVALID_ARGUMENT", _key);
            return false;
        }
        // search for key
        const char* keyPos = strstr(_cmd, _key);
        if (!keyPos) {
            this->sendError("EXTRACT_KEY_NOT_FOUND", _key);
            return false;
        }
        // move to value start
        keyPos += strlen(_key);
        if (*keyPos != '=') {
            this->sendError("EXTRACT_INVALID_KEY_FORMAT", _key);
            return false;
        }
        keyPos++; // skip '='
        // find end of values
        const char* endPos = keyPos;
        while (*endPos != ' ' && *endPos != '\0' && *endPos != '>') {
            endPos++;
        }
        // calc length
        size_t len = endPos - keyPos;
        if (len >= _outSize - 1) {
            this->sendError("EXTRACT_VALUE_TOO_LONG", _key);
            return false;
        }
        // copy value string
        strncpy(_out, keyPos, len);
        _out[len] = '\0';
        return true;
    }

    bool extractAngles(char* _anglesStr, float _outAngles[POSE_ANGLE_COUNT]) {
        char* token = strtok(_anglesStr, ";");
        for (int i = 0; i < POSE_ANGLE_COUNT; i++) {
            if (token == nullptr || token[0] == '\0') {
                this->sendError("TOO_FEW_ANGLES");
                return false;
            }
            _outAngles[i] = atof(token);
            token = strtok(nullptr, ";");
        }
        return true;
    }

    bool parseSetPose(const char* _cmd, ParsedCommand& _outCmd) {
        // extract id
        char idStr[MAX_ID_LENGTH] = "";
        if (!this->extractField(_cmd, "id", idStr, sizeof(idStr))) {
            return false;
        }
        // extract angles
        char anglesStr[MAX_ANGLE_LENGTH] = "";
        if (!this->extractField(_cmd, "angles", anglesStr, sizeof(anglesStr))) {
            return false;
        }
        _outCmd.id = atoi(idStr);
        
        // parse angles
        if (!this->extractAngles(anglesStr, _outCmd.angles)) {
            return false;
        }
        _outCmd.hasAngles = true;
        return true;
    }

    bool parseGetPose(const char* _cmd, ParsedCommand& _outCmd) {
        // extract id
        char idStr[MAX_ID_LENGTH] = "";
        if (!this->extractField(_cmd, "id", idStr, sizeof(idStr))) {
            return false;
        }
        _outCmd.id = atoi(idStr);
        return true;
    }

    bool parseGoTo(const char* _cmd, ParsedCommand& _outCmd) {
        // extract id
        char idStr[MAX_ID_LENGTH] = "";
        if (!this->extractField(_cmd, "id", idStr, sizeof(idStr))) {
            return false;
        }
        _outCmd.id = atoi(idStr);
        return true;
    }

    bool parseStop(const char* _cmd, ParsedCommand& _outCmd) {
        // no args
        (void)_cmd;
        (void)_outCmd;
        return true;
    }

public:
    SerialInterface(Stream& _serial)
        : m_serial(_serial) {}

    // poll serial for incoming commands, return true if a command was processed
    bool poll(ParsedCommand& _outCmd) {
        // if nothing to read, return false
        if (!m_serial.available()) {
            return false;
        }

        // clear output command
        _outCmd = ParsedCommand();

        // read line, expecting newline termination
        String line = m_serial.readStringUntil('\n');
        line.trim();

        // strip <> and convert to C string
        if (line.length() < 2 || line[0] != '<' || line[line.length() - 1] != '>') {
            this->sendError("MISSING_ANGLE_BRACKETS");
            return true;
        }
        line = line.substring(1, line.length() - 1);
        const char* cmd = line.c_str();

        // switch exec on command type
        _outCmd.type = this->identifyType(cmd);
        bool valid = false;
        switch (_outCmd.type) {
            case CMD_SET_POSE:
                valid = this->parseSetPose(cmd, _outCmd);
                break;
            case CMD_GET_POSE:
                valid = this->parseGetPose(cmd, _outCmd);
                break;
            case CMD_GO_TO:
                valid = this->parseGoTo(cmd, _outCmd);
                break;
            case CMD_STOP:
                valid = this->parseStop(cmd, _outCmd);
                break;
            default:
                this->sendError("UNKNOWN_COMMAND");
                valid = false;
                break;
        }

        // set validity flag
        _outCmd.valid = valid;
        return true;
    }

    void sendAccepted(id_t _id) {
        m_serial.print("<ACCEPTED ");
        m_serial.print(_id);
        m_serial.println(">");
    }

    void sendRejected(id_t _id, const char* _reason) {
        m_serial.print("<REJECTED ");
        m_serial.print(_id);
        m_serial.print(" ");
        m_serial.print(_reason);
        m_serial.println(">");
    }

    void sendReturnPose(id_t _id, const Pose& _pose) {
        m_serial.print("<POSE ");
        m_serial.print(_id);
        m_serial.print(" ");
        for (int i = 0; i < POSE_ANGLE_COUNT; ++i) {
            if (i > 0) {
                m_serial.print(";");
            }
            m_serial.print(_pose.angles[i], 5);
        }
        m_serial.println(">");
    }

    void sendCompleted(id_t _id) {
        m_serial.print("<COMPLETED ");
        m_serial.print(_id);
        m_serial.println(">");
    }

    void sendStopped(id_t _id) {
        m_serial.print("<STOPPED ");
        m_serial.print(_id);
        m_serial.println(">");
    }

    void sendError(const char* _reason, const char* _details = nullptr) {
        m_serial.print("<ERROR ");
        m_serial.print(_reason);
        if (_details) {
            m_serial.print(": ");
            m_serial.print(_details);
        }
        m_serial.println(">");
    }

};
