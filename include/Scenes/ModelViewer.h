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

    float lineThickness = 1.05f;
    int vertices{};
    int faces{};
    
    bool drawOutline = false;

    public:
    ModelViewer();
    void InitializeScene();
    void DrawScene();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);

    int GetVertices();
    int GetFaces();
};

#endif