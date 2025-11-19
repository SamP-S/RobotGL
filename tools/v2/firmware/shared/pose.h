#pragma once

#define POSE_STORAGE 256
#define POSE_ANGLES 5

// 5 DOF logical pose of the robot arm
struct Pose {
    float angles[POSE_ANGLES];
};
