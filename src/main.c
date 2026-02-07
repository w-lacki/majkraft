#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include <cglm/cglm.h>

void resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window,GLFW_KEY_W)) {
        input.movement[2] = -1.0f;
    } else if (glfwGetKey(window,GLFW_KEY_S)) {
        input.movement[2] = 1.0f;
    }

    if (glfwGetKey(window,GLFW_KEY_A)) {
        input.movement[0] = -1.0f;
    } else if (glfwGetKey(window,GLFW_KEY_D)) {
        input.movement[0] = 1.0f;
    }

    if (glfwGetKey(window,GLFW_KEY_SPACE)) {
        input.movement[1] = 1.0f;
    } else if (glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)) {
        input.movement[1] = -1.0f;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    input_update((vec2){(float) xpos, (float) ypos});
}

void set_uniform_mat4f(Shader shader, const char *name, mat4 model) {
    const GLint modelLoc = glGetUniformLocation(shader.id, name);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *) model);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Majkraft", nullptr, nullptr);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    glEnable(GL_DEPTH_TEST);

    //######################
    constexpr float vertices[] = {
        // positions          // texture coords

        // Front face
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        // Back face
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

        // Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        // Right face
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

        // Top face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

        // Bottom face
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f
    };

    constexpr unsigned int indices[] = {
        0, 1, 3, 1, 2, 3, // front
        4, 5, 7, 5, 6, 7, // back
        8, 9, 11, 9, 10, 11, // left
        12, 13, 15, 13, 14, 15, // right
        16, 17, 19, 17, 18, 19, // top
        20, 21, 23, 21, 22, 23 // bottom
    };
    //######################


    const Shader *shader = create_shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    if (!shader) return -1;

    const Texture *texture_dirt = load_texture("./textures/dirt.png");
    if (!texture_dirt) return -1;
    // #########################################

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    // #############################################
    use_shader(shader);

    mat4 view;
    glm_mat4_identity(view);
    glm_translate(view, (vec3){0.0f, 1.0f, 0.0f});


    mat4 projection;
    glm_perspective(
        glm_rad(90.0f),
        1280 / 720.0f,
        0.1f,
        100.0f,
        projection
    );


    glfwSetCursorPosCallback(window, mouse_callback);

    set_uniform_mat4f(*shader, "projection", projection);
    char title[256];
    double last_update_time = glfwGetTime();
    double last_title_upate = last_update_time;
    while (!glfwWindowShouldClose(window)) {
        float update_time = glfwGetTime();
        float dt = update_time - last_update_time;
        last_update_time = update_time;

        process_input(window);
        camera_view_update();
        camera_movement_update(dt);
        input_reset();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        glm_mat4_identity(view);
        glm_rotate(view, glm_rad(camera.rotation[1]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(view, glm_rad(camera.rotation[0]), (vec3){0.0f, 1.0f, 0.0f});
        vec3 inv_position = {-camera.position[0], -camera.position[1], -camera.position[2]};
        glm_translate(view, inv_position);
        set_uniform_mat4f(*shader, "view", view);

        use_texture(texture_dirt);

        mat4 model;
        glm_mat4_identity(model);
        set_uniform_mat4f(*shader, "model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwPollEvents();
        glfwSwapBuffers(window);

        if (update_time - last_title_upate > 0.1) {
            sprintf(title, "X: %.2f Y: %.2f Z: %.2f YAW: %.2f PITCH: %.2f FPS: %d", camera.position[0],
                    camera.position[1],
                    camera.position[2], camera.rotation[0], camera.rotation[1], (int) (1.0 / dt));
            glfwSetWindowTitle(window, title);
            last_title_upate = update_time;
        }
    }


    delete_texture(texture_dirt);
    delete_shader(shader);
    glfwTerminate();
    return 0;
}
