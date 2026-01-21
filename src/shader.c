#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

Shader *create_shader(const char *vertex_path, const char *fragment_path) {
    char source_file_buffer[1024];

    FILE *file = fopen(vertex_path, "r");
    size_t read_bytes = fread(source_file_buffer, 1, 1024, file);
    fclose(file);

    source_file_buffer[read_bytes] = '\0';
    const GLchar *source_ptr = source_file_buffer;
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &source_ptr, nullptr);
    glCompileShader(vertex_shader);

    int success;
    char error_message[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, error_message);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", error_message);
        return nullptr;
    }

    file = fopen(fragment_path, "r");
    read_bytes = fread(source_file_buffer, 1, 1024, file);
    fclose(file);

    source_file_buffer[read_bytes] = '\0';
    source_ptr = source_file_buffer;
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const GLchar *const*) &source_ptr, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, error_message);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s\n", error_message);
        return nullptr;
    }

    const GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, error_message);
        printf("ERROR::SHADER::FRAGMENT::LINKING_FAILED %s\n", error_message);
        return nullptr;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    Shader *shader = malloc(sizeof(Shader));
    shader->id = shader_program;

    return shader;
}

void delete_shader(Shader *shader) {
    glDeleteProgram(shader->id);
    free(shader);
}

void use_shader(Shader *shader) {
    glUseProgram(shader->id);
}
