#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "stb_image.h"

#include "Shader.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

unsigned int VAO{};
unsigned int VBO{};
unsigned int EBO{};
unsigned int shaderProgram{};
unsigned int texture{};
unsigned int texture2{};

// Texturing.

int width{};
int height{};
int channels{};

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

// float colorVerts[] = {
//     // position.             // color.
//     0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
//     -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
//     0.0f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f
// };

float texVerts[] = {
    // position.             // color.           // texcoords.
    0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,     0.0f, 1.0f
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
    // glBufferData(GL_ARRAY_BUFFER, sizeof(colorVerts), colorVerts, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texVerts), texVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers.

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);                    // position.
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);                    // colored verts pos.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                       // tex verts pos.
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // color.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));     // tex verts color.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));     // texture.
    glEnableVertexAttribArray(2);

    // Texturing.

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../Assets/Textures/testTex.png", &width, &height, &channels, 3);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture." << std::endl;
    }

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../Assets/TExtures/container.png", &width, &height, &channels, 3);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);

    // Process shaders.

    Shader* testShader = new Shader();
    testShader->CreateFromFile("Shaders/testVert.glsl", "Shaders/testFrag.glsl");
    testShader->UseShader();
    testShader->SetInt("texSampler1", 0);
    testShader->SetInt("texSampler2", 1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop.

    while(!glfwWindowShouldClose(mainWindow))
    {
        ProcessInput(mainWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);

        // glDrawArrays(GL_TRIANGLES, 0, 3);                    // tri
        // glDrawArrays(GL_TRIANGLES, 0, 6);                    // bow
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    // rec

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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