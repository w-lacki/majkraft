#include "camera.h"

Camera camera = {0};

void camera_view_update() {
    if (!input.delta[0] && !input.delta[1]) return;

    vec2 delta;
    glm_vec2_copy(input.delta, delta);

    glm_vec2_mul(delta, (vec2){SENSITIVITY, SENSITIVITY}, delta);

    camera.rotation[0] += delta[0];
    camera.rotation[1] += delta[1];

    if (camera.rotation[1] > MAX_PITCH) {
        camera.rotation[1] = MAX_PITCH;
    } else if (camera.rotation[1] < -MAX_PITCH) {
        camera.rotation[1] = -MAX_PITCH;
    }
}

void camera_movement_update(double dt) {
    if (input.movement[2]) {
        camera.position[0] += sinf(glm_rad(camera.rotation[0])) * -1.0f * input.movement[2] * MOVE_SPEED * dt;
        camera.position[2] += cosf(glm_rad(camera.rotation[0])) * input.movement[2] * MOVE_SPEED * dt;
    }

    if (input.movement[0]) {
        camera.position[0] += sinf(glm_rad(camera.rotation[0] - 90.0f)) * -1.0f * input.movement[0] * MOVE_SPEED * dt;
        camera.position[2] += cosf(glm_rad(camera.rotation[0] - 90.0f)) * input.movement[0] * MOVE_SPEED * dt;
    }

    camera.position[1] += input.movement[1] * JUMP_SPEED * dt;
}
