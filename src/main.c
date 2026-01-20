#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *VERTEX_SHADER_SRC = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec4 vertexColor;\n"
        "void main(){\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   vertexColor = gl_Position;"
        "}";
const char *FRAGMENT_SHADER_SRC = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 vertexColor;\n"
        "void main(){\n"
        "FragColor = vertexColor;\n"
        "}";

void resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, resize_callback);


    //######################

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
   };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    //######################

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SRC, nullptr);
    glCompileShader(vertex_shader);
    int success;
    char error_message[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, error_message);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", error_message);
        return -1;
    }

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SRC, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, error_message);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s\n", error_message);
        return -1;
    }

    const GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, error_message);
        printf("ERROR::SHADER::FRAGMENT::LINKING_FAILED %s\n", error_message);
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    // #########################################

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    // #############################################
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}
