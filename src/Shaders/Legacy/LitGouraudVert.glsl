#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1f;
vec3 ambient = objColor * ambientStrength;
float specSTR = 0.01f;

out vec3 color;

void main()
{
    vec3 vertPos = vec3(projection * view * model * vec4(aPos, 1.0));
    vec3 norm = normalize(norm * aNorm);

    vec3 lightDir = normalize(lightPos - vertPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specSTR * spec * lightColor;

    vec3 light = ambient + diffuse + specular;
    color = objColor * light;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}