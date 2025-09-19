--Vertex--
#version 330 core
layout (location = 0) vec3 aPos;
layout (location = 1) vec3 aTexCoord;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texCoord = aTexCoord;
}

--Fragment--
#version 330 core

struct Material
{
    sampler2D texture1;
    sampler2D texture2;
};

in vec2 texCoord;
out vec4 FragColor;

uniform Material matr;

void main()
{
    FragColor = mix(texture(matr.texture1, texCoord), texture(matr.texture2, texCoord), 1.0);
}