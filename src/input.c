#include "input.h"

Input input = {0};
void input_update(vec2 curr) {
    glm_vec2_copy(input.mouse_curr, input.mouse_prev);
    glm_vec2_copy(curr, input.mouse_curr);
    glm_vec2_sub(input.mouse_curr, input.mouse_prev, input.delta);
}

void input_reset() {
    glm_vec2_zero(input.delta);
    glm_vec3_zero(input.movement);
}