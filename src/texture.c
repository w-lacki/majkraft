#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture *load_texture(const char *path) {
    int width, height, channels;
    stbi_uc *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        printf("Failed to load texture from %s\n", path);
        return nullptr;
    }

    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    Texture *texture = malloc(sizeof(Texture));
    texture->id = id;
    return texture;
}

void delete_texture(Texture *texture) {
    glDeleteTextures(1, &texture->id);
    free(texture);
}

void use_texture(Texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}
