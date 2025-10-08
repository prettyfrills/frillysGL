#ifndef FUNCTIONGRAPH_H
#define FUNCTIONGRAPH_H

#include "Scene.h"

class Model;
class Shader;

class FunctionGraph : public Scene
{
    private:
    Model* pointModel{};
    Shader* shader{};
    int resolution{};
    float spacing{};
    float speed{};
    float size{};
    float time{};

    public:
    void InitializeScene();
    void DrawScene();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);
    void SetTime(float t);
};

#endif