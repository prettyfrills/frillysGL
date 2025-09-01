#include "Model.h"
#include "stb_image.h"

// Model::Model(const char* directory)
// {
//     LoadModel(directory);
// }

Model::Model(std::string const &directory)
{
    LoadModel(directory);
}

Model::~Model()
{
    // while(meshes.size() > 0)
    //     delete meshes[0];
}

void Model::Draw(Shader& shader)
{
    for(Mesh mesh : meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(std::string const &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process node meshes.
    for(int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    
    // Process node children.
    for(int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices{};
    std::vector<Texture> textures{};
    std::vector<unsigned int> indices{};

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        // Process position, normals, and texcoord.
        Vertex vertex{};

        glm::vec3 v{};
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.position = v;

        v.x = mesh->mNormals[i].x;
        v.y = mesh->mNormals[i].y;
        v.z = mesh->mNormals[i].z;
        vertex.normal = v;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 tex{};
            tex.x = mesh->mTextureCoords[0][i].x;
            tex.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = tex;
        }
        else
            vertex.texCoord = glm::vec2(0.0f);

        vertices.push_back(vertex);
    }

    // Process indices.
    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process material data.
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffMap = LoadTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffMap.begin(), diffMap.end());
        std::vector<Texture> specMap = LoadTextures(mat, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specMap.begin(), specMap.end());
    }

    return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::LoadTextures(aiMaterial* material, aiTextureType textureType, std::string typeName)
{
    std::vector<Texture> texList{};

    for(int i = 0; i < material->GetTextureCount(textureType); i++)
    {
        aiString str{};
        material->GetTexture(textureType, i, &str);
        bool skip = false;

        for(int j = 0; j < textures.size(); j++)
        {
            if(std::strcmp(textures[j].path.data(), str.C_Str()) == 0)
            {
                texList.push_back(textures[j]);
                skip = true;
                break;
            }
        }

        if(!skip)
        {
            Texture texture{};
            texture.ID = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
        }
    }

    return texList;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string fileName = std::string(path);
    fileName = directory + '/' + fileName;
    unsigned int textureID{};
    glGenTextures(1, &textureID);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        GLenum format{};
        if(channels == 1)
            format = GL_RED;
        else if(channels == 3)
            format = GL_RGB;
        else if(channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
    }
    else
        std::cout << "Texture failed to load at path: " << path << std::endl;

    stbi_image_free(data);
    return textureID;
}