#include "Shader.h"


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

void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
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

void Shader::AddShader(GLuint program, const char* shaderContent, GLenum shaderType)
{
    unsigned int shader = glCreateShader(shaderType);
    const char* shaderCode[1];
    shaderCode[0] = shaderContent;
    int shaderLen[1];
    // shaderLen[0] = strlen(shaderContent);

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
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
        return;
    }


}

Shader::~Shader()
{
    ClearShader();
}