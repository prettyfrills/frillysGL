#ifndef FUNCTIONGRAPH_H
#define FUNCTIONGRAPH_H

#include "Scene.h"
#include <functional>

class Model;
class Shader;

typedef std::function<float (float, float, float)> libfunc;

enum shape
{
    WAVE,
    MULTIWAVE,
    RIPPLE
};

class FunctionLibrary
{
    private:
    libfunc currentFunction{};

    public:
    FunctionLibrary();
    void SwitchFunction(shape inShape);
    static float Wave(float x, float t, float speed);
    static float Multiwave(float x, float t, float speed);
    static float Ripple(float x, float t, float speed);
    float GetResult(float x, float t, float speed);
};

class FunctionGraph : public Scene
{
    private:
    FunctionLibrary* funclib{};
    Model* pointModel{};
    Shader* shader{};
    int resolution{};
    float spacing{};
    float speed{};
    float size{};
    float time{};

    public:
    FunctionGraph();
    void InitializeScene();
    void DrawScene();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);
    void SetTime(float t);
};

#endif