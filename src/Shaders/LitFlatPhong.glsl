--Vertex--
#version 330 core
layout (location = 0) vec3 aPos;
layout (location = 1) vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm;

out vec3 Normal;
out vec3 fragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));
    Normal = norm * aNorm;       // Transform mesh normal to world space.
}

--Fragment--
#version 330 core

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambStrength = 0.1f;
float spec specStrength = 0.01f;
vec3 ambient = objColor * ambStrength;

in vec3 outNorm;
in vec3 fragPos;
out vec4 FragColor;

vec3 norm = normalize(Normal);

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0);
    float spec = max(dot(viewDir, reflectDir), 0);
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;
    vec3 light = ambient + diffuse + specular;

    FragColor = objColor * light;
}