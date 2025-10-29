#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/ext/matrix_clip_space.hpp"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Scenes/ModelViewer.h"
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "Defaults.h"

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
bool mouseFocused = true;
bool wireframe = false;

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

    const float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
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

    // Make scene.
    testScene->InitializeScene();

    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable features.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Framebuffer.
    unsigned int framebuffer{};
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Generate texture.
    unsigned int colorTex{};
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

    // Create render buffer for depth and stencil framebuffer.
    unsigned int renderObj{};
    glGenRenderbuffers(1, &renderObj);
    glBindRenderbuffer(GL_RENDERBUFFER, renderObj);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObj);

    // Finalize.
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE))
        std::cerr << "ERROR::FRAMEBUFFER: Framebuffer incomplete." << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create quad.
    unsigned int vao{};
    unsigned int vbo{};
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultMesh::Quad), DefaultMesh::Quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);   // pos.
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // texcoord.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader fbShader = Shader("src/Shaders/Postprocessing/Framebuffer.glsl");
    fbShader.UseShader();
    fbShader.SetInt("screenTex", 0);

    // Skybox.
    std::string textureFaces[] = {
        "res/Textures/Skybox/right.jpg",
        "res/Textures/Skybox/left.jpg",
        "res/Textures/Skybox/top.jpg",
        "res/Textures/Skybox/bottom.jpg",
        "res/Textures/Skybox/front.jpg",
        "res/Textures/Skybox/back.jpg"
    };

    unsigned int skybox{};
    glGenTextures(1, &skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

    int width, height, channels;
    unsigned char* data;
    stbi_set_flip_vertically_on_load(false);
    for(int i = 0; i < 6; i++)
    {
        data = stbi_load(textureFaces[i].c_str(), &width, &height, &channels, 0);
        if(!data){
            std::cerr << "Failed to load texture: " << textureFaces[i] << std::endl;
            return 1;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    stbi_image_free(data);

    // Create skybox mesh.
    unsigned int cubeVAO{};
    unsigned int cubeVBO{};
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultMesh::skyboxVertices), DefaultMesh::skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Shader skyShader = Shader("src/Shaders/Skybox.glsl");
    Camera* cam = testScene->camera;

    // Render loop.
    while(!glfwWindowShouldClose(mainWindow))
    {
        ProcessInput(mainWindow);

        // Start ImGUI frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Draw scene to framebuffer.
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        testScene->SetDeltaTime(deltaTime);
        testScene->DrawScene();
        testScene->DrawMenu();

        // Draw skybox.
        glDepthMask(GL_FALSE);
        skyShader.UseShader();
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);
        view = glm::mat4(glm::mat3(cam->GetLookAt()));
        projection = glm::perspective(glm::radians(60.0f), ((float)windowWidth / (float)windowHeight) ,0.1f, 100.0f);
        skyShader.SetView(view);
        skyShader.SetProjection(projection);
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        // Draw quad with framebuffer texture.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        fbShader.UseShader();
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Render ImGUI UI.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &framebuffer);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete testScene;
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
    }

    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;
    lastX = xPos;
    lastY = yPos;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    if(mouseFocused)
    testScene->RotateCamera(xOffset, yOffset);
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