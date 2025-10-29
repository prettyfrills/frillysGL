--Vertex--
#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
out vec3 TexCoords;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

--Fragment--
#version 330 core

in vec3 TexCoords;
uniform samplerCube skybox;
out vec4 FragColor;

void main()
{
    FragColor = texture(skybox, TexCoords);
}