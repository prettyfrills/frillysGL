#ifndef SCENE_H
#define SCENE_H

#include "glm/glm.hpp"
class Model;
class Shader;
class Camera;

class Scene
{
    protected:
    Camera* camera{};

    public:
    virtual void InitializeScene() = 0;
    virtual void DrawScene() = 0;
    virtual void MoveCamera(glm::vec3 direction) = 0;
    virtual void RotateCamera(float x, float y) = 0;
};

#endif