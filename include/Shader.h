#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    private:

    unsigned int ID{};          // Shader program ID.

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
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
};

#endif