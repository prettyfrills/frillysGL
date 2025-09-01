#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

class Model;
class Shader;

class Scene
{
    private:
    std::vector<Model*> models{};

    public:
    void AddModel(std::string path);
    void DrawScene(Shader& shader);
};

#endif