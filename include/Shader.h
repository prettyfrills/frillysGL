#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

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
    void UseShader();
    void ClearShader();

    // Uniform Utils.
    unsigned int GetID();
    unsigned int GetModel();
    unsigned int GetView();
    unsigned int GetProjection();
    unsigned int GetNormal();

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec3(const std::string &name, float x, float y, float z);
    void SetVec3(const std::string &name, glm::vec3 vector);
};

#endif