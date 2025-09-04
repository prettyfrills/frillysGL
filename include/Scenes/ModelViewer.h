#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include "Scene.h"

class Model;
class Shader;

class ModelViewer : public Scene
{
    public:
    Model* sceneModel;
    Shader* shader;

    ModelViewer();
    void InitializeScene();
    void DrawScene();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);
};

#endif