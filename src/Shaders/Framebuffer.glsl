--Vertex--
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoord;
}

--Fragment--
#version 330 core
uniform sampler2D screenTex;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    FragColor = texture(screenTex, TexCoords);
}