#include "Scenes/FunctionGraph.h"
#include "Model.h"
#include "Camera.h"

#include <cmath>
#include <cstdlib>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#pragma region FunctionLibrary

FunctionLibrary::FunctionLibrary()
{
    currentFunction = Ripple;
}

float FunctionLibrary::Wave(float x, float t, float speed)
{
    return std::sin(x + (t * speed));
}

float FunctionLibrary::Multiwave(float x, float t, float speed)
{
    float y = std::sin(x + (t * speed));
    y += 0.5f * std::sin(2.0f * (x + (t * speed)));
    return y * (2.0f / 3.0f);
}

float FunctionLibrary::Ripple(float x, float t, float speed)
{
    float d = std::abs(x);
    float y = std::sin(4.0f * d + (t * speed));
    return y;
}

void FunctionLibrary::SwitchFunction(shape inShape)
{
    switch(inShape)
    {
        case WAVE:
        currentFunction = Wave;
        break;

        case MULTIWAVE:
        currentFunction = Multiwave;
        break;

        case RIPPLE:
        currentFunction = Ripple;
        break;
    }
}

float FunctionLibrary::GetResult(float x, float t, float speed)
{
    if(currentFunction)
        return currentFunction(x, t, speed);
    else
        return 0.0f;   
}

#pragma endregion

FunctionGraph::FunctionGraph()
{
    funclib = new FunctionLibrary();
}

void FunctionGraph::InitializeScene()
{
    camera = new Camera();
    pointModel = new Model("res/Models/Cube/Cube.obj");
    shader = new Shader();
    shader->CreateFromFile("src/Shaders/ColorPos.glsl");
    shader->UseShader();
    resolution = 10;
    spacing = 3.0f;
    speed = 2.0f;
    size = 0.5f;

    camera->Move(glm::vec3(-7.0f, 0.0f, -10.0f));
}

void FunctionGraph::DrawScene()
{
    // Configure stencil buffer.
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);    // Enable writing to stencil buffer.

    shader->UseShader();

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = camera->GetLookAt();
    projection = glm::perspective(glm::radians(60.0f), 800.0f/600.0f, 0.1f, 100.0f);
    shader->SetView(view);
    shader->SetProjection(projection);

    for(int i = 0; i < resolution; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 pos = glm::vec3(0.0f);
        pos.x = i * spacing;
        pos.y = funclib->GetResult(i, time, speed);
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(size));
        shader->SetModel(model);
        pointModel->Draw(*shader);
    }
}

void FunctionGraph::MoveCamera(glm::vec3 direction)
{
    camera->Move(direction);
}

void FunctionGraph::RotateCamera(float x, float y)
{
    camera->Rotate(x, y);
}

void FunctionGraph::SetTime(float t)
{
    time = t;
}