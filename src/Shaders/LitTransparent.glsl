--Vertex--
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm;

out vec2 texCoord;
out vec3 fragPos;
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));
    Normal = norm * aNorm;
    texCoord = aTexCoord;
}

--Fragment--
#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

const int nPointLights = 4;

in vec2 texCoord;
in vec3 fragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[nPointLights];
uniform Material matr;

out vec4 FragColor;

vec4 DirLighting(DirectionalLight light, vec3 normal, vec3 viewDir);
vec4 PointLighting(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec4 light = DirLighting(dirLight, norm, viewDir);
    for(int i = 0; i < nPointLights; i++)
        light += PointLighting(pointLights[i], norm, viewDir, fragPos);

    // Discard fragments which have an alpha value below a threshold of 1.
    if(light.a < 1)
        discard;

    FragColor = light;
}

vec4 DirLighting(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);

    vec4 ambient = vec4(light.ambient, 1.0) * texture(matr.texture_diffuse1, texCoord);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(matr.texture_diffuse1, texCoord);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(matr.texture_specular1, texCoord);

    return (ambient + diffuse + specular);
}

vec4 PointLighting(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);
    float distance = length(light.position - fragPos);
    float attenuation = 1 / ((light.constant) + (light.linear * distance) + (light.quadratic * (distance * distance)));

    vec4 ambient = vec4(light.ambient, 1.0) * texture(matr.texture_diffuse1, texCoord);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(matr.texture_diffuse1, texCoord);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(matr.texture_specular1, texCoord);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}