#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "Camera.h"
#include "Shader.h"
#include "Lights.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void KeyDownCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void ProcessInput(GLFWwindow* window);
void ToggleMouseLock(GLFWwindow* window);

// TODO: Refactor into mesh class.
unsigned int VAO{};
unsigned int VBO{};
unsigned int EBO{};

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
Camera* camera = new Camera();

// Input variables.
float lastX = 400.0f;
float lastY = 300.0f;
float deltaTime = 0.0f;
float prevTime = 0.0f;
float sensitivity = 0.1f;
bool mouseFocused = false;

unsigned int lightVAO{};
glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, -5.0f);

// Texturing.
int width{};
int height{};
int channels{};

float litTexCube[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 lightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main()
{
    // Initialize glfw.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "FrillysGL", NULL, NULL);
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
    glfwSetCursorPosCallback(mainWindow, MouseCallback);
    glfwSetKeyCallback(mainWindow, KeyDownCallback);

    // Create and bind buffers. Sends vertex data to the GPU.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(litTexCube), litTexCube, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Process shaders.

    Shader* testShader = new Shader();
    testShader->CreateFromFile("Shaders/LitTexVert.glsl", "Shaders/MultiLightFrag.glsl");
    testShader->UseShader();

    // TODO: Relocate texturing code from Shader class to mesh.
    // Set texture indices and roughness.
    testShader->SetInt("matr.diffuse", 0);
    testShader->SetInt("matr.specular", 1);
    testShader->SetFloat("matr.roughness", 2.0f);

    // Directional light.
    // DirectionalLight* dirLight = new DirectionalLight(glm::vec3(0.0f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.05f), glm::vec3(-0.2f, -1.0f, -0.3f));
    // testShader->AddDirectionalLight(dirLight);

    // Multiple point lights.
    for(int i = 0; i < 4; i++)
    {
        PointLight* light = new PointLight(lightPositions[i], glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.05f), 1.0f, 0.09f, 0.032f);
        testShader->AddPointLight(light, i);
    }

    // Spot light.
    // testShader->SetVec3("lght.position", camera->position);
    // testShader->SetVec3("lght.direction", camera->forward);
    // testShader->SetFloat("lght.cutoff", glm::cos(glm::radians(12.5f)));
    // testShader->SetFloat("lght.outer", glm::cos(glm::radians(17.5f)));
    // testShader->SetVec3("lght.diffuse", glm::vec3(1.0f));
    // testShader->SetVec3("lght.specular", glm::vec3(0.05f));
    // testShader->SetFloat("lght.constant", 1.0f);
    // testShader->SetFloat("lght.linear", 0.09f);
    // testShader->SetFloat("lght.quadratic", 0.032f);

    testShader->AddTexture("../res/Textures/container2.png", width, height, channels);
    testShader->AddTexture("../res/Textures/containerSpec.png", width, height, channels);

    Shader* lightShader = new Shader();
    lightShader->CreateFromFile("Shaders/lightCubeVert.glsl", "Shaders/lightCubeFrag.glsl");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Render loop.

    while(!glfwWindowShouldClose(mainWindow))
    {
        ProcessInput(mainWindow);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render lit objects.
        testShader->UseShader();
        testShader->SetVec3("viewPos", camera->position);

        // testShader->SetVec3("lght.position", camera->position);      // Spotlight.
        // testShader->SetVec3("lght.direction", camera->forward);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = camera->GetLookAt();
        projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(testShader->GetView(), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(testShader->GetProjection(), 1, GL_FALSE, glm::value_ptr(projection));

        testShader->UseTextures();

        glBindVertexArray(VAO);
        for(int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat3 norm = glm::mat3(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float rot = 20.0f * i;
            model = glm::rotate(model, glm::radians(rot), glm::vec3(1.0f, 0.3f, 0.5f));
            norm = glm::transpose(glm::inverse(model));
            glUniformMatrix4fv(testShader->GetModel(), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix3fv(testShader->GetNormal(), 1, GL_FALSE, glm::value_ptr(norm));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader->UseShader();

        glUniformMatrix4fv(lightShader->GetView(), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lightShader->GetProjection(), 1, GL_FALSE, glm::value_ptr(projection));
        lightShader->SetVec3("color", glm::vec3(1.0f));
        glBindVertexArray(lightVAO);

        for(int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(lightShader->GetModel(), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if(!mouseFocused)
    {
        lastX = xPos;
        lastY = yPos;
        mouseFocused = true;
    }

    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;
    lastX = xPos;
    lastY = yPos;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera->Rotate(xOffset, yOffset);
}

void KeyDownCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if(key == GLFW_KEY_F8 && action == GLFW_PRESS)
    {
        ToggleMouseLock(window);
    }
}

void ProcessInput(GLFWwindow* window)
{
    glm::vec3 velocity = glm::vec3(0.0f);
    float frameTime = glfwGetTime();
    deltaTime = frameTime - prevTime;
    prevTime = frameTime;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        velocity.z += 1.0f;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        velocity.z -= 1.0f;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        velocity.x += 1.0f;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        velocity.x -= 1.0f;
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        velocity.y += 1.0f;
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        velocity.y -= 1.0f;

    camera->Move(velocity * deltaTime);
}

void ToggleMouseLock(GLFWwindow* window)
{
    if(!mouseFocused)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseFocused = true;
        return;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    mouseFocused = false;
}