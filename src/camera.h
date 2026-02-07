#ifndef MAJKRAFT_CAMERA_H
#define MAJKRAFT_CAMERA_H

#include "input.h"

#define MAX_PITCH 90.0f
#define SENSITIVITY 0.1f
#define MOVE_SPEED 7.0f
#define JUMP_SPEED 5.0f

typedef struct Camera Camera;

struct Camera {
    vec2 rotation;
    vec3 position;
};

extern Camera camera;

void camera_view_update();
void camera_movement_update(double dt);

#endif //MAJKRAFT_CAMERA_H
