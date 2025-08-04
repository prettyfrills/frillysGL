#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Vertex{
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 texCoord{};
};

struct Texture{
    unsigned int ID{};
    std::string type{};
};

class Shader;

class Mesh
{
    private:
    unsigned int VAO{};
    unsigned int VBO{};
    unsigned int EBO{};

    void InitializeMesh();

    public:
    std::vector<Vertex> vertices{};
    std::vector<Texture> textures{};
    std::vector<unsigned int> indices{};

    Mesh();
    Mesh(std::vector<Vertex> inVerts, std::vector<Texture> inTex, std::vector<unsigned int> inIndices);
    void Draw(Shader& shader);
};

#endif