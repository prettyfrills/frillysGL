#include "Scene.h"
#include "Model.h"

void Scene::AddModel(std::string path)
{
    Model* model = new Model("path");
    models.push_back(model);
}

void Scene::DrawScene(Shader& shader)
{
    for(Model* model : models)
    {
        model->Draw(shader);
    }
}