#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "Shader.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

unsigned int VAO{};
unsigned int VBO{};
unsigned int EBO{};
unsigned int vert{};
unsigned int frag{};
unsigned int shaderProgram{};

const char* vertCode = 
"#version 330 core                      \n"
"layout (location = 0) in vec3 aPos;    \n"
"layout (location = 1) in vec3 aColor;  \n"
"out vec4 vertCol;                      \n"
"void main()                            \n"
"{                                      \n"
"gl_Position = vec4(aPos, 1.0);         \n"
"vertCol = vec4(aColor, 1.0);           \n"
"}\0";

const char* fragCode =
"#version 330 core                          \n"
"in vec4 vertCol;                           \n"
"out vec4 FragColor;                        \n"
"void main()                                \n"
"{                                          \n"
"FragColor = vertCol;                       \n"
"}\0";

float vertices[] = {
    0.5f, 0.5f, 0.0f,       // Top right.
    0.5f, -0.5f, 0.0f,      // Bottom right.
    -0.5f, -0.5f, 0.0f,     // Bottom left.
    -0.5f, 0.5f, 0.0f
};

// float bowVerts[] = {
//     -0.5f, -0.5f, 0.0f,
//     -0.5f, 0.5f, 0.0f,
//     0.0f, 0.0f, 0.0f,

//     0.0f, 0.0f, 0.0f,
//     0.5f, 0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f
// };

float colorVerts[] = {
    // position             // color.
    0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 3,                // Triangle 1.
    1, 2, 3                 // Triangle 2.
};

int main()
{
    // Initialize glfw.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);
    if(!mainWindow)
    {
        std::cerr << "Failed to create glfw window. Aborting." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(mainWindow);

    // Initialize glad.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL functions with GLAD. Aborting." << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(mainWindow, framebufferSizeCallback);

    // Create and bind buffers. Sends vertex data to the GPU.

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(bowVerts), bowVerts, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorVerts), colorVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers.

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Process shaders.

    Shader* testShader = new Shader();
    testShader->CreateFromString(vertCode, fragCode);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop.

    while(!glfwWindowShouldClose(mainWindow))
    {
        ProcessInput(mainWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // float time = glfwGetTime();
        // float newCol = (sin(time) / 2.0f) + 0.5f;
        // int colLocation = glGetUniformLocation(shaderProgram, "globalCol");
        // glUniform4f(colLocation, 0.0f, newCol, 0.0f, 1.0f);
        testShader->UseShader();

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}