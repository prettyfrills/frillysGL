#include "Scenes/Scene.h"
#include "Camera.h"

Scene::~Scene()
{
    delete camera;
}

void Scene::InitializeScene()
{
    // Load models, create shaders, initialize rendering order.
    // Should be overwritten by scene classes which inherit from this one.
}

void Scene::DrawScene()
{
    // Should be overwritten by scene classes which inherit from this one.
}

void Scene::MoveCamera(glm::vec3 direction)
{
    // Should be overwritten by scene classes which inherit from this one.
}