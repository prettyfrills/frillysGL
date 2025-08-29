#include <glm/glm.hpp>

class Light
{
    protected:
    glm::vec3 position{};
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};

    public:

    Light();
    Light(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec);
    glm::vec3 GetPosition();
    glm::vec3 GetAmbient();
    glm::vec3 GetDiffuse();
    glm::vec3 GetSpecular();
    void SetPosition(glm::vec3 newPos);
};

class DirectionalLight : public Light
{
    protected:
    glm::vec3 direction{};

    public:
    DirectionalLight();
    DirectionalLight(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec, glm::vec3 inDir);
    glm::vec3 GetDirection();
};

class PointLight : public Light
{
    protected:
    // Attenuation.
    float constant{};
    float linear{};
    float quadratic{};

    public:
    PointLight();
    PointLight(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec, float inConst, float inLin, float inQuad);

    float GetConstant();
    float GetLinear();
    float GetQuadratic();
};

class SpotLight : public Light
{
    protected:
    // Attenuation.
    float constant{};
    float linear{};
    float quadratic{};

    float inner{};
    float outer{};

    glm::vec3 direction{};

    public:
    SpotLight();
    SpotLight(glm::vec3 inPos, glm::vec3 inAmb, glm::vec3 inDiff, glm::vec3 inSpec, glm::vec3 inDir, float inConst, float inLin, float inQuad, float innerRad, float outerRad);

    float GetConstant();
    float GetLinear();
    float GetQuadratic();
    float GetInnerRadius();
    float GetOuterRadius();

    glm::vec3 GetDirection();
};