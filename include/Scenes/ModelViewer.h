#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include "Scene.h"

class Model;
class Shader;

class ModelViewer : public Scene
{
    private:
    Model* sceneModel{};
    Model* lightModel{};
    Model* grassModel{};
    Shader* shader{};
    Shader* stencil{};
    Shader* light{};

    float deltaTime{};
    int vertices{};
    int faces{};
    float FPS{};

    float lineThickness = 1.05f;    
    bool drawOutline = false;

    public:
    ModelViewer();
    void InitializeScene();
    void DrawScene();
    void DrawMenu();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);
    void Outline(bool enabled);

    void SetDeltaTime(float delta);
    int GetVertices();
    int GetFaces();
};

#endif