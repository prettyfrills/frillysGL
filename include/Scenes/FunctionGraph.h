#ifndef FUNCTIONGRAPH_H
#define FUNCTIONGRAPH_H

#include "Scene.h"
#include <functional>
#include <string>

class Model;
class Shader;

typedef std::function<float (float, float, float)> libfunc;

class FunctionLibrary
{
    private:
    libfunc currentFunction{};

    public:
    FunctionLibrary();
    void SwitchFunction(std::string inShape);
    static float Wave(float x, float t, float speed);
    static float Multiwave(float x, float t, float speed);
    static float Ripple(float x, float t, float speed);
    float GetResult(float x, float t, float speed);
};

class FunctionGraph : public Scene
{
    private:
    FunctionLibrary* funclib{};
    const char* selectedFunc{};
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
    void DrawMenu();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);
    void SetTime(float t);
    void ChangeFunction(std::string inVal);
};

#endif