// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <chrono>

// Shaders
#include "Shaders/main_vertex.glsl"
#include "Shaders/main_fragment.glsl"

void add_main_shaders(GLuint shaderProgram) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        fprintf(stderr, "Failed to compile vertex shader\n");
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        fprintf(stderr, "Failed to compile fragment shader\n");
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
}

#define RED_LITERAL_FLOATS 1.0f, 0.0f, 0.0f
#define BLUE_LITERAL_FLOATS 0.0f, 0.0f, 1.0f
#define GREEN_LITERAL_FLOATS 0.0f, 1.0f, 0.0f

int main(int argc, char* argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    GLuint vertex_array_ref;
    glGenVertexArrays(1, &vertex_array_ref);
    glBindVertexArray(vertex_array_ref);
    GLuint vertex_buffer_ref;
    glGenBuffers(1, &vertex_buffer_ref); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ref);
    GLuint shaderProgram = glCreateProgram();
    add_main_shaders(shaderProgram);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    constexpr GLint vertex_attrib_stride = 5 * sizeof(float);
    constexpr GLint vertex_position_length = 2 * sizeof(float);
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, vertex_attrib_stride, 0);
    glEnableVertexAttribArray(posAttrib);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, vertex_attrib_stride, (void*)vertex_position_length);
    glEnableVertexAttribArray(colorAttrib);

    auto t_start = std::chrono::high_resolution_clock::now();

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float vertices[] = {
         0.0f,  0.5f, RED_LITERAL_FLOATS,   // Vertex 1 (X, Y)
         0.5f, -0.5f, GREEN_LITERAL_FLOATS, // Vertex 2 (X, Y)
        -0.5f, -0.5f, BLUE_LITERAL_FLOATS,  // Vertex 3 (X, Y)
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 3);



        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
