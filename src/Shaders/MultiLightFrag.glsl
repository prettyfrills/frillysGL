#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float roughness;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D tex1;
uniform vec3 viewPos;
uniform Material matr;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 DirLighting(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 PointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Get directional light component, then add light from all point lights.
    vec3 light = DirLighting(dirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        light += PointLighting(pointLights[i], norm, fragPos, viewDir);
    }

    FragColor = vec4(light, 1.0);
}

vec3 DirLighting(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);

    vec3 ambient = light.ambient * vec3(texture(matr.diffuse, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(matr.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(matr.specular, texCoord));

    return (ambient + diffuse + specular);
}

vec3 PointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-light.position, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(matr.diffuse, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(matr.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(matr.specular, texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}