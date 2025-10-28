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
out vec4 FragColor;
uniform sampler2D screenTex;

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),      // top-left.
        vec2(0.0, offset),          // top.
        vec2(offset, offset),       // top-right.
        vec2(-offset, 0.0),         // middle-left.
        vec2(0.0, 0.0),             // middle.
        vec2(offset, 0.0),          // middle-right.
        vec2(-offset, -offset),     // bottom-left.
        vec2(0.0, -offset),         // bottom.
        vec2(offset, -offset)       // bottom-right.
    );

    // Sharpen.
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1, 9, -1,
    //     -1, -1, -1
    // );

    // Blur.
    // float kernel[9] = float[](
    //     1.0/16, 2.0/16, 1.0/16,
    //     2.0/16, 4.0/16, 2.0/16,
    //     1.0/16, 2.0/16, 1.0/16
    // );

    // Edge detection.
    float kernel[9] = float[](
        1.0, 1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0, 1.0, 1.0
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
        sampleTex[i] = vec3(texture(screenTex, TexCoords.st + offsets[i]));

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];

    FragColor = vec4(color, 1.0);
}