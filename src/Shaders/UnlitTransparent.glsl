--Vertex--
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
    texCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

--Fragment--
#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float roughness;
};

uniform Material matr;
in vec2 texCoord;
out vec4 FragColor;

void main()
{
    vec4 color = texture(matr.texture_diffuse1, texCoord);
    if(color.a < 0.1)

    FragColor = 
}