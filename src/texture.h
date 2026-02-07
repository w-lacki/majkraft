#ifndef MAJKRAFT_TEXTURE_H
#define MAJKRAFT_TEXTURE_H
#include "glad/glad.h"

typedef struct Texture Texture;

struct Texture {
    GLuint id;
};

Texture *load_texture(const char *path);

void delete_texture(Texture *texture);

void use_texture(Texture *texture);

#endif //MAJKRAFT_TEXTURE_H
