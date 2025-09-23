--Vertex--
#version 330 core
layout (location = 0) vec3 aPos;
layout (location = 1) vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambStrength = 0.1f;
float specStrength = 0.01f;
vec3 ambient = objColor * ambStrength;

out vec3 color;

void main()
{
    vec3 vertPos = vec3(projection * view * model * vec4(aPos, 1.0));
    vec3 normal = normalize(norm * aNorm)

    vec3 lightDir = normalize(lightPos - vertPos);
    vec3 viewDir = normalize(viewPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0);
    float spec = max(dot(viewDir, reflectDir), 0);
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;
    vec3 light = ambient + diffuse + specular;

    color = objColor * light;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

--Fragment--
#version 330 core
in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0);
}