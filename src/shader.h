#ifndef MAJKRAFT_SHADER_H
#define MAJKRAFT_SHADER_H
#include "glad/glad.h"

typedef struct shader Shader;

struct shader {
    GLuint id;
};

Shader *create_shader(const char *vertex_path, const char *fragment_path);
void delete_shader(Shader *shader);

void use_shader(Shader *shader);

#endif //MAJKRAFT_SHADER_H
