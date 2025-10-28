--Vertex--
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}

--Fragment--
#version 330 core

in vec2 TexCoords;
uniform sampler2D screenTex;

out vec4 FragColor;

void main()
{
    FragColor = texture(screenTex, TexCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
    FragColor = vec4(average, average, average, 1.0);
}