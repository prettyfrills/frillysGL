#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float roughness;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;

    // spotlight
    float cutoff;
    float outer;

    // attenuation.
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;
out vec4 FragColor;

uniform Material matr;
uniform Light lght;
uniform vec3 viewPos;

vec3 ambient = lght.ambient * vec3(texture(matr.diffuse, texCoord));
vec3 norm = normalize(Normal);

void main()
{
    // Point light.
    // float distance = length(lght.position - fragPos);
    // float attenuation = 1 / (lght.constant + lght.linear * distance + lght.quadratic * (distance * distance));

    // vec3 lightDir = normalize(lght.position - fragPos);
    // float diff = max(dot(norm, lightDir), 0);
    // vec3 diffuse = lght.diffuse * diff * vec3(texture(matr.diffuse, texCoord));

    // vec3 viewDir = normalize(viewPos - fragPos);
    // vec3 reflectDir = reflect(-lght.position, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);
    // vec3 specular = lght.specular * spec * vec3(texture(matr.specular, texCoord));

    // // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;

    // Spot light.
    float distance = length(lght.position - fragPos);
    float attenuation = 1 / (lght.constant + lght.linear * distance + lght.quadratic * (distance * distance));

    vec3 lightDir = normalize(lght.position - fragPos);
    
    float theta = dot(lightDir, normalize(-lght.direction));
    float epsilon = lght.cutoff - lght.outer;
    float intensity = clamp((theta - lght.outer) / epsilon, 0.0, 1.0);
    vec3 light;

    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = lght.diffuse * diff * vec3(texture(matr.diffuse, texCoord));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);
    vec3 specular = lght.specular * spec * vec3(texture(matr.specular, texCoord));

    diffuse *= intensity;
    specular *= intensity;
    light = diffuse + specular + ambient;
    light *= attenuation;

    FragColor = vec4(light, 1.0);
}