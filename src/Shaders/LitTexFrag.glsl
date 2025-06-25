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
};

in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D tex1;
uniform Material matr;
uniform Light lght;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 ambient = lght.ambient * vec3(texture(matr.diffuse, texCoord));
vec3 norm = normalize(Normal);

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = lght.diffuse * diff * vec3(texture(matr.diffuse, texCoord));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightPos, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);
    vec3 specular = lght.specular * spec * vec3(texture(matr.specular, texCoord));

    vec3 light = ambient + diffuse + specular;
    FragColor = vec4(light, 1.0);
}