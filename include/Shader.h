#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Light;
class DirectionalLight;
class PointLight;
class SpotLight;

class Shader
{
    private:

    unsigned int ID{};          // Shader program ID.
    unsigned int uniModel{};
    unsigned int uniView{};
    unsigned int uniProjection{};
    unsigned int uniNorm{};

    void AddShader(GLuint program, const char* shaderContent, GLenum shaderType);
    void CompileShader(const char* vertexCode, const char* fragmentCode);

    public:

    Shader();
    ~Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFile(const char* vertexPath, const char* fragmentPath);
    void CreateFromFile(const char* path);

    void AddDirectionalLight(DirectionalLight* light);
    void AddPointLight(PointLight* light, int index);
    void AddSpotLight(SpotLight* light);

    void UseShader();
    void ClearShader();

    // Uniform Utils.
    unsigned int GetID();
    unsigned int GetModel();
    unsigned int GetView();
    unsigned int GetProjection();
    unsigned int GetNormal();
    void SetModel(glm::mat4 model);
    void SetView(glm::mat4 view);
    void SetProjection(glm::mat4 projection);
    void SetNormal(glm::mat3 norm);

    void SetBool(const char* name, bool value) const;
    void SetInt(const char* name, int value) const;
    void SetFloat(const char* name, float value) const;
    void SetVec3(const char* name, float x, float y, float z);
    void SetVec3(const char* name, glm::vec3 vector);

    void SetBool(std::string, bool value) const;
    void SetInt(std::string name, int value) const;
    void SetFloat(std::string name, float value) const;
    void SetVec3(std::string name, float x, float y, float z);
    void SetVec3(std::string name, glm::vec3 vector);
};

#endif