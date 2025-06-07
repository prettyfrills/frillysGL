#version 330 core

in vec4 vertCol;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

void main()
{
    // FragColor = vertCol;
    // FragColor = texture(texSampler1, texCoord);
    FragColor = mix(texture(texSampler1, texCoord), texture(texSampler2, texCoord), 0.5);
}