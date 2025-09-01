#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float roughness;
};

in vec2 texCoord;
out vec4 FragColor;

uniform Material matr;

void main()
{
    FragColor = texture(matr.texture_diffuse1, texCoord);
}