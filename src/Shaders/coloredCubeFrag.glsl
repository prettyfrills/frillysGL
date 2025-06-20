#version 330 core

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 fragPos;

float ambientStrength = 0.1f;
vec3 ambient = ambientStrength * lightColor;
vec3 norm = normalize(Normal);
float specSTR = 0.01f;

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightPos, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specSTR * spec * lightColor;

    vec3 light = ambient + diffuse + specular;
    FragColor = vec4(objColor * light, 1.0);
}