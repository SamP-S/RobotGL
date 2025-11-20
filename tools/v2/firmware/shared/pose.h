#pragma once

#define POSE_BUFFER_SIZE 256
#define POSE_ANGLE_COUNT 5

// 5 DOF logical pose of the robot arm
struct Pose {
    float angles[POSE_ANGLE_COUNT];
};
