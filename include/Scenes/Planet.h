#ifndef PLANET_H
#define PLANET_H
#include "Scenes/Scene.h"

class DirectionalLight;

class PlanetScene : public Scene
{
    private:
    Model* planetModel{};
    Shader* planetShader{};
    Shader* skyShader{};
    DirectionalLight* light{};
    unsigned int skyVAO, skyVBO;
    unsigned int skyTex{};

    void LoadSkybox();

    public:
    PlanetScene();
    void InitializeScene();
    void DrawScene();
    void MoveCamera(glm::vec3 direction);
    void RotateCamera(float x, float y);
};

#endif