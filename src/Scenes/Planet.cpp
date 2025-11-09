#include "Scenes/Planet.h"
#include "Defaults.h"
#include "Lights.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/matrix.hpp"
#include "stb_image.h"

std::string textures[] = {
        "res/Textures/Skybox/right.jpg",
        "res/Textures/Skybox/left.jpg",
        "res/Textures/Skybox/top.jpg",
        "res/Textures/Skybox/bottom.jpg",
        "res/Textures/Skybox/front.jpg",
        "res/Textures/Skybox/back.jpg"
    };

PlanetScene::PlanetScene()
{
    
}

void PlanetScene::LoadSkybox()
{
    // Assemble skybox mesh.
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultMesh::skyboxVertices), DefaultMesh::skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Load skybox textures.
    glGenTextures(1, &skyTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex);

    int width, height, channels;
    unsigned char* data{};
    stbi_set_flip_vertically_on_load(false);
    for(int i = 0; i < 6; i++)
    {
        data = stbi_load(textures[i].c_str(), &width, &height, &channels,0);
        if(!data){return;}
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    stbi_image_free(data);
}

void PlanetScene::InitializeScene()
{
    camera = new Camera();
    planetModel = new Model("res/Models/uvSphere/uvSphere.obj");
    planetShader = new Shader("src/Shaders/Planet.glsl");
    skyShader = new Shader("src/Shaders/Skybox.glsl");
    LoadSkybox();

    planetShader->UseShader();
    planetShader->SetVec3("objColor", glm::vec3(1.0f, 0.569f, 0.643f));
    planetShader->SetVec3("lightPos", glm::vec3(3.0f, 1.0f, 3.0f));
    planetShader->SetVec3("lightColor", glm::vec3(1.0f));
    planetShader->SetVec3("ambientColor", glm::vec3(1.0f));

    // TODO: Add framebuffer for postprocess support.
}

void PlanetScene::DrawScene()
{
    // Draw planet.
    planetShader->UseShader();
    planetShader->SetVec3("viewPos", camera->position);
    // planetShader->SetVec3("color", glm::vec3(1.0f, 0.569f, 0.643f));

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    glm::mat3 norm(1.0f);

    view = camera->GetLookAt();
    projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f,0.1f, 100.0f);
    norm = glm::transpose(glm::inverse(model));
    planetShader->SetTransformation(model, norm, view, projection);
    planetModel->Draw(*planetShader);

    // Draw skybox.
    glDepthMask(GL_FALSE);
    skyShader->UseShader();
    view = glm::mat4(glm::mat3(camera->GetLookAt()));
    skyShader->SetView(view);
    skyShader->SetProjection(projection);

    glBindVertexArray(skyVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void PlanetScene::MoveCamera(glm::vec3 direction)
{
    camera->Move(direction);
}

void PlanetScene::RotateCamera(float x, float y)
{
    camera->Rotate(x, y);
}