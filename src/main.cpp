#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

unsigned int VAO{};
unsigned int VBO{};
unsigned int vert{};
unsigned int frag{};
unsigned int shaderProgram{};

const char* vertCode = 
"#version 330 core                      \n"
"layout (location = 0) in vec3 aPos;    \n"
"void main()                            \n"
"{                                      \n"
"gl_Position = vec4(aPos, 1.0);         \n"
"}\0";

const char* fragCode =
"#version 330 core                          \n"
"out vec4 FragColor;                        \n"
"void main()                                \n"
"{                                          \n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);  \n"
"}\0";

float vertices[] = {
    -0.5f, -0.5f, 0,
    0.5f, -0.5f, 0,
    0.0f, 0.5f, 0.0f
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
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers.

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Process shaders.
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertCode, NULL);
    glCompileShader(vert);

    int vStatus{};
    char vLog[512];
    glGetShaderiv(vert, GL_COMPILE_STATUS, &vStatus);
    if(!vStatus)
    {
        glGetShaderInfoLog(vert, 512, NULL, vLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vLog <<std::endl;
    }

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragCode, NULL);
    glCompileShader(frag);

    int fStatus{};
    char fLog[512];
    glGetShaderiv(frag, GL_COMPILE_STATUS, &fStatus);
    if(!fStatus)
    {
        glGetShaderInfoLog(frag, 512, NULL, fLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fLog <<std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);

    glDeleteShader(vert);
    glDeleteShader(frag);

    // Render loop.

    while(!glfwWindowShouldClose(mainWindow))
    {
        ProcessInput(mainWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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