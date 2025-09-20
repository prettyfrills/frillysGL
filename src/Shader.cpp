#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
//#include <sstream>
#include <string>
#include "Lights.h"

Shader::Shader()
: ID(0)
{}

void Shader::UseShader()
{
    glUseProgram(ID);
}

void Shader::ClearShader()
{
    if(ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

#pragma region Get/Set

unsigned int Shader::GetID()
{
    return ID;
}

unsigned int Shader::GetModel()
{
    return uniModel;
}

unsigned int Shader::GetView()
{
    return uniView;
}

unsigned int Shader::GetProjection()
{
    return uniProjection;
}

unsigned int Shader::GetNormal()
{
    return uniNorm;
}

void Shader::SetModel(glm::mat4 model)
{
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::SetView(glm::mat4 view)
{
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void Shader::SetProjection(glm::mat4 projection)
{
    glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::SetNormal(glm::mat3 norm)
{
    glUniformMatrix3fv(uniNorm, 1, GL_FALSE, glm::value_ptr(norm));
}

void Shader::SetBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::SetInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetVec3(const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::SetVec3(const char* name, glm::vec3 vector)
{
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vector));
}

void Shader::SetBool(std::string name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(std::string name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(std::string name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(std::string name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVec3(std::string name, glm::vec3 vector)
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}

#pragma endregion Get/Set

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* path)
{
    std::string vertCode{};
    std::string fragCode{};
    std::ifstream fileStream(path, std::ios::in);
    std::string codeLine{};

    if(!fileStream)
    {
        std::cerr << "Failed to read shader file. Aborting." << std::endl;
        return;
    }

    bool writeFrag = false;
    while(!fileStream.eof())
    {
        std::getline(fileStream, codeLine);
        if(codeLine == "--Vertex--")
            continue;
        if(codeLine == "--Fragment--")
        {
            writeFrag = true;
            continue;
        }

        if(!writeFrag)
            vertCode.append(codeLine + '\n');
        else
            fragCode.append(codeLine + '\n');
    }

    const char* cVert = vertCode.c_str();
    const char* cFrag = fragCode.c_str();
    CompileShader(cVert, cFrag);
}

void Shader::CreateFromFile(const char* vertexPath, const char* fragmentPath)
{
    std::string vertShader{};
	std::string fragShader{};
	std::ifstream vertStream(vertexPath, std::ios::in);
	std::ifstream fragStream(fragmentPath, std::ios::in);
	std::string vertLine{};
	std::string fragLine{};

    if(!vertStream)
    {
        std::cerr << "Failed to read vertex shader. Aborting." << std::endl;
        return;
    }
    if(!fragStream)
    {
        std::cerr << "Failed to read fragment shader. Aborting." << std::endl;
        return;
    }

    while(!vertStream.eof())
    {
        std::getline(vertStream, vertLine);
        vertShader.append(vertLine + '\n');
    }
    vertStream.close();
    while(!fragStream.eof())
    {
        std::getline(fragStream, fragLine);
        fragShader.append(fragLine + '\n');
    }
    fragStream.close();

    const char* cVert = vertShader.c_str();
    const char* cFrag = fragShader.c_str();
    CompileShader(cVert, cFrag);
}

void Shader::AddDirectionalLight(DirectionalLight* light)
{
    SetVec3("dirLight.direction", light->GetDirection());
    SetVec3("dirLight.ambient", light->GetAmbient());
    SetVec3("dirLight.diffuse", light->GetDiffuse());
    SetVec3("dirLight.specular", light->GetSpecular());
}

void Shader::AddPointLight(PointLight* light, int index)
{
    std::string i = std::to_string(index);
    std::string pos = "pointLights[" + i + "].position";
    std::string amb = "pointLights[" + i + "].ambient";
    std::string diff = "pointLights[" + i + "].diffuse";
    std::string spec = "pointLights[" + i + "].specular";
    std::string con = "pointLights[" + i + "].constant";
    std::string lin = "pointLights[" + i + "].linear";
    std::string quad = "pointLights[" + i + "].quadratic";

    SetVec3(pos, light->GetPosition());
    SetVec3(amb, light->GetAmbient());
    SetVec3(diff, light->GetDiffuse());
    SetVec3(spec, light->GetSpecular());
    SetFloat(con, light->GetConstant());
    SetFloat(lin, light->GetLinear());
    SetFloat(quad, light->GetQuadratic());
}

void Shader::AddSpotLight(SpotLight* light)
{
    SetVec3("lght.position", light->GetPosition());
    SetVec3("lght.direction", light->GetDirection());
    SetFloat("lght.cutoff", glm::cos(glm::radians(light->GetInnerRadius())));
    SetFloat("lght.outer", glm::cos(glm::radians(light->GetOuterRadius())));
    SetVec3("lght.diffuse", light->GetDiffuse());
    SetVec3("lght.specular", light->GetSpecular());
    SetFloat("lght.constant", light->GetConstant());
    SetFloat("lght.linear", light->GetLinear());
    SetFloat("lght.quadratic", light->GetQuadratic());
}

void Shader::AddShader(GLuint program, const char* shaderContent, GLenum shaderType)
{
    unsigned int shader = glCreateShader(shaderType);
    const char* shaderCode[1];
    shaderCode[0] = shaderContent;
    int shaderLen[1];

    glShaderSource(shader, 1, shaderCode, NULL);
    glCompileShader(shader);

    // Validate shader.
    int result{};
    char errorLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(shader, 512, NULL, errorLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << errorLog << std::endl;
        return;
    }

    // If compile succeeded, attach shader to program then delete.
    glAttachShader(program, shader);
    glDeleteShader(shader);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    ID = glCreateProgram();
    if (!ID)
	{
		printf("Failed to create shader program. Aborting.");
		return;
	}
    AddShader(ID, vertexCode, GL_VERTEX_SHADER);
    AddShader(ID, fragmentCode, GL_FRAGMENT_SHADER);
    glLinkProgram(ID);

    // Validate linking and program in OpenGL context.
    int result{};
    char errorLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(ID, 512, NULL, errorLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
        return;
    }

    glValidateProgram(ID);
	glGetProgramiv(ID, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(ID, 512, NULL, errorLog);
        std::cerr << "ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n" << errorLog << std::endl;
        return;
    }

    uniModel = glGetUniformLocation(ID, "model");
    uniView = glGetUniformLocation(ID, "view");
    uniProjection = glGetUniformLocation(ID, "projection");
    uniNorm = glGetUniformLocation(ID, "norm");
}

Shader::~Shader()
{
    ClearShader();
}