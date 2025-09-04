#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Lights.h"

#include "Scenes/ModelViewer.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void KeyDownCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void ProcessInput(GLFWwindow* window);
void DrawUtilsMenu();
void ToggleMouseLock(GLFWwindow* window);
void ToggleWireframe();

int windowWidth = 1600;
int windowHeight = 1200;

// Input variables.
float lastX = 400.0f;
float lastY = 300.0f;
float deltaTime = 0.0f;
float prevTime = 0.0f;
float sensitivity = 0.1f;
bool mouseFocused = false;
bool wireframe = false;

// Debug utils.
float FPS{};
int modelFaces{};
int modelVertices{};

// glm::vec3 lightPositions[] = {
//     glm::vec3( 0.7f,  0.2f,  2.0f),
//     glm::vec3( 2.3f, -3.3f, -4.0f),
//     glm::vec3(-4.0f,  2.0f, -12.0f),
//     glm::vec3( 0.0f,  0.0f, -3.0f)
// };

// TODO: Fix paths when loading model and shader from scene class.
// Camera* camera = new Camera();
ModelViewer* testScene = new ModelViewer();

int main()
{
    // Initialize glfw.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(windowWidth, windowHeight, "FrillysGL", NULL, NULL);
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

    float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    glViewport(0, 0, windowWidth * mainScale, windowHeight * mainScale);
    glfwSetFramebufferSizeCallback(mainWindow, FramebufferSizeCallback);
    glfwSetCursorPosCallback(mainWindow, MouseCallback);
    glfwSetKeyCallback(mainWindow, KeyDownCallback);

    // Initialize ImGUI.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init();

    // Make models.
    testScene->InitializeScene();
    Model* testModel = new Model(std::string("res/Backpack/backpack.obj"));
    Model* lightModel = new Model(std::string("res/Models/Cube/Cube.obj"));

    modelFaces = testModel->GetFaces();
    modelVertices = testModel->GetVertices();

    // Process shaders.
    Shader* testShader = new Shader();
    testShader->CreateFromFile("src/Shaders/LitTexVert.glsl", "src/Shaders/MultiLightModelFrag.glsl");
    testShader->UseShader();
    testShader->SetFloat("matr.roughness", 2.0f);

    Shader* lightShader = new Shader();
    lightShader->CreateFromFile("src/Shaders/lightCubeVert.glsl", "src/Shaders/UnlitTexFrag.glsl");

    // Multiple point lights.
    // for(int i = 0; i < 4; i++)
    // {
    //     PointLight* light = new PointLight(lightPositions[i], glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.05f), 1.0f, 0.09f, 0.032f);
    //     testShader->AddPointLight(light, i);
    // }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Render loop.
    while(!glfwWindowShouldClose(mainWindow))
    {
        ProcessInput(mainWindow);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start ImGUI frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawUtilsMenu();

        // Render lit objects.
        // testShader->UseShader();
        // testShader->SetVec3("viewPos", camera->position);

        // glm::mat4 model = glm::mat4(1.0f);
        // glm::mat3 norm = glm::mat3(1.0f);
        // glm::mat4 view = glm::mat4(1.0f);
        // glm::mat4 projection = glm::mat4(1.0f);
        // view = camera->GetLookAt();
        // norm = glm::transpose(glm::inverse(model));
        // projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // testShader->SetModel(model);
        // testShader->SetNormal(norm);
        // testShader->SetView(view);
        // testShader->SetProjection(projection);

        // testModel->Draw(*testShader);

        testScene->DrawScene();

        // Render lights.
        // lightShader->UseShader();
        // lightShader->SetView(view);
        // lightShader->SetProjection(projection);
        // lightShader->SetVec3("color", glm::vec3(1.0f));

        // for(int i = 0; i < 4; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, lightPositions[i]);
        //     model = glm::scale(model, glm::vec3(0.2f));
        //     lightShader->SetModel(model);
        //     lightModel->Draw(*lightShader);
        // }

        // Render ImGUI UI.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
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

    testScene->RotateCamera(xOffset, yOffset);
    // camera->Rotate(xOffset, yOffset);
}

void KeyDownCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if(key == GLFW_KEY_F8 && action == GLFW_PRESS)
        ToggleMouseLock(window);

    if(key == GLFW_KEY_Z && action == GLFW_PRESS)
        ToggleWireframe();
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

    testScene->MoveCamera(velocity * deltaTime);
    // camera->Move(velocity * deltaTime);
}

void DrawUtilsMenu()
{
    FPS = 1 / deltaTime;
    ImGui::Begin("Stats");
    ImGui::Text("Frame rate (FPS): %f", FPS);
    ImGui::Text("Time between frames: %f", deltaTime);

    ImGui::Text("Model:");
    ImGui::Text("Faces: %d", modelFaces);
    ImGui::Text("Vertices: %d", modelVertices);
    // TODO: Display vertex count. Display face count.

    ImGui::End();
}

void ToggleMouseLock(GLFWwindow* window)
{
    if(!mouseFocused)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    mouseFocused = !mouseFocused;
}

void ToggleWireframe()
{
    if(!wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    wireframe = !wireframe;
}