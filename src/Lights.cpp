#include "Lights.h"

#pragma region Light

Light::Light()
:   position(glm::vec3(0.0f, 0.0f, -5.0f)), ambient(glm::vec3(0.2f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(0.05f))
    {}

Light::Light(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec)
:   position(inPos), ambient(inAmb), diffuse(inDiff), specular(inSpec)
    {}

glm::vec3 Light::GetPosition()
{
    return position;
}

glm::vec3 Light::GetAmbient()
{
    return ambient;
}

glm::vec3 Light::GetDiffuse()
{
    return diffuse;
}

glm::vec3 Light::GetSpecular()
{
    return specular;
}

void Light::SetPosition(glm::vec3 newPos)
{
    position = newPos;
}

#pragma endregion Light

#pragma region DirectionalLight

DirectionalLight::DirectionalLight()
: Light(), direction(glm::vec3(-0.2f, -1.0f, -0.3f))
{}

DirectionalLight::DirectionalLight(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec, glm::vec3 inDir)
: Light(inPos, inAmb, inDiff, inSpec), direction(inDir)
{}

glm::vec3 DirectionalLight::GetDirection()
{
    return direction;
}

#pragma endregion DirectionalLight

#pragma region PointLight

PointLight::PointLight()
: Light(), constant(1.0f), linear(0.09f), quadratic(0.032f)
{}

PointLight::PointLight(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec, float inConst, float inLin, float inQuad)
: Light(inPos, inAmb, inDiff, inSpec), constant(inConst), linear(inLin), quadratic(inQuad)
{}

float PointLight::GetConstant()
{
    return constant;
}

float PointLight::GetLinear()
{
    return linear;
}

float PointLight::GetQuadratic()
{
    return quadratic;
}

#pragma endregion PointLight

#pragma region SpotLight

SpotLight::SpotLight()
: Light(), constant(1.0f), linear(0.09f), quadratic(0.032f), direction(glm::vec3(0.0f)), inner(12.5f), outer(17.5f)
{}

SpotLight::SpotLight(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec, glm::vec3 inDir, float inConst, float inLin, float inQuad, float innerRad, float outerRad)
: Light(inPos, inAmb, inDiff, inSpec), direction(inDir), constant(inConst), linear(inLin), quadratic(inQuad), inner(innerRad), outer(outerRad)
{}

float SpotLight::GetConstant()
{
    return constant;
}

float SpotLight::GetLinear()
{
    return linear;
}

float SpotLight::GetQuadratic()
{
    return quadratic;
}

float SpotLight::GetInnerRadius()
{
    return inner;
}

float SpotLight::GetOuterRadius()
{
    return outer;
}

glm::vec3 SpotLight::GetDirection()
{
    return direction;
}

#pragma endregion SpotLight