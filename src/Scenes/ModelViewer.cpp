#include "Scenes/ModelViewer.h"
#include "Model.h"
#include "Lights.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

glm::vec3 lightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  3.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

ModelViewer::ModelViewer()
{
    //InitializeScene();
}

void ModelViewer::InitializeScene()
{
    camera = new Camera();
    sceneModel = new Model("res/Backpack/backpack.obj");

    stencil = new Shader();
    stencil->CreateFromFile("src/Shaders/Outline.glsl");
    stencil->UseShader();
    stencil->SetVec3("color", glm::vec3(1.0f, 0.569f, 0.643f));

    shader = new Shader();
    shader->CreateFromFile("src/Shaders/LitTexPhong.glsl");
    shader->UseShader();
    shader->SetFloat("matr.roughness", 32.0f);

    // Add lights to shader.
    for(int i = 0; i < 4; i++)
    {
        PointLight* light = new PointLight(lightPositions[i], glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.05f), 1.0f, 0.09f, 0.032f);
        shader->AddPointLight(light, i);
    }

    lightModel = new Model("res/Models/Cube/Cube.obj");
    light = new Shader();
    light->CreateFromFile("src/Shaders/UnlitFlat.glsl");

    grassModel = new Model("res/Models/Window/Window.obj", false);
}

void ModelViewer::DrawScene()
{
    // Configure stencil buffer.
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);    // Enable writing to stencil buffer.

    shader->UseShader();
    shader->SetVec3("viewPos", camera->position);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat3 norm = glm::mat3(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Draw primary model.
    view = camera->GetLookAt();
    norm = glm::transpose(glm::inverse(model));
    projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    shader->SetModel(model);
    shader->SetNormal(norm);
    shader->SetView(view);
    shader->SetProjection(projection);
    sceneModel->Draw(*shader);

    // Draw lights.
    light->UseShader();
    light->SetView(view);
    light->SetProjection(projection);
    light->SetVec3("color", glm::vec3(1.0f, 0.569f, 0.643f));

    for(int i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        light->SetModel(model);
        lightModel->Draw(*light);
    }

    // Draw grass.
    shader->UseShader();

    for(glm::vec3 pos : lightPositions)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        shader->SetModel(model);
        grassModel->Draw(*shader);
    }

    if(!drawOutline)
        return;

    // Draw outline.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);    // Disable writing to the stencil buffer.
    glDisable(GL_DEPTH_TEST);
    stencil->UseShader();
    model = glm::scale(model, glm::vec3(lineThickness));
    stencil->SetModel(model);
    stencil->SetNormal(norm);
    stencil->SetView(view);
    stencil->SetProjection(projection);
    sceneModel->Draw(*stencil);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void ModelViewer::DrawMenu()
{
    ImGui::Begin("Stats");
    ImGui::Text("FPS: %f", FPS);
    ImGui::Text("Time between frames: %f", deltaTime);

    ImGui::Text("Faces: %d", faces);
    ImGui::Text("Vertices: %d", vertices);
    ImGui::End();
}

void ModelViewer::MoveCamera(glm::vec3 direction)
{
    camera->Move(direction);
}

void ModelViewer::RotateCamera(float x, float y)
{
    camera->Rotate(x, y);
}

void ModelViewer::Outline(bool enabled)
{
    drawOutline = enabled;
}

int ModelViewer::GetVertices()
{
    return sceneModel->GetVertices();
}

int ModelViewer::GetFaces()
{
    return sceneModel->GetFaces();
}

void ModelViewer::SetDeltaTime(float delta)
{
    deltaTime = delta;
    FPS = 1 / delta;
}