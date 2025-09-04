#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include "Mesh.h"
#include "Shader.h"

class Model
{
    private:
    std::vector<Texture> textures{};
    std::vector<Mesh> meshes{};
    std::string directory{};
    int vertices{};
    int faces{};

    void LoadModel(std::string const &directory);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadTextures(aiMaterial* material, aiTextureType textureType, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory);

    public:
    Model(const char* directory);
    Model(std::string const &directory);
    ~Model();
    void Draw(Shader& shader);

    int GetVertices();
    int GetFaces();
};

#endif