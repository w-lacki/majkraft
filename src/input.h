#ifndef MAJKRAFT_INPUT_H
#define MAJKRAFT_INPUT_H
#include <cglm/cglm.h>

typedef struct Input Input;

struct Input {
    vec2 mouse_prev;
    vec2 mouse_curr;
    vec2 delta;
    vec3 movement;
};

extern Input input;

void input_update(vec2 current);

void input_reset();

#endif //MAJKRAFT_INPUT_H
