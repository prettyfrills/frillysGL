#include "Mesh.h"
#include "Shader.h"
#include "glad/glad.h"

Mesh::Mesh()
{
    
}

Mesh::Mesh(std::vector<Vertex> inVerts, std::vector<Texture> inTex, std::vector<unsigned int> inIndices)
    : vertices(inVerts), textures(inTex), indices(inIndices)
{
    InitializeMesh();
}

void Mesh::InitializeMesh()
{
    // Assign data to buffers then send to GPU.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Indexed drawing
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Assign and enable vertex attributes. Can use offsetof macro because data in structs are sequential.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));     // pos.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));       // norm.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));     // texcoord.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Unbind VAO.
    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
    // Assume texture naming convention texture_typeN.
    unsigned int diffuseN = 1;
    unsigned int specularN = 1;

    for(int i = 0; i < textures.size(); i++)
    {

        glActiveTexture(GL_TEXTURE0 + i);

        // Get N from texture name.
        std::string number{};
        std::string name = textures[i].type;

        if(name == "texture_diffuse")
            number = std::to_string(diffuseN++);
        else if(name == "texture_specular")
            number = std::to_string(specularN++);

        shader.SetInt("matr." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    // Draw mesh.
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}