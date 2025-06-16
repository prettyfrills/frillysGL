#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

void main()
{
    FragColor = mix(texture(texSampler1, texCoord), texture(texSampler2, texCoord), 1.0);
}