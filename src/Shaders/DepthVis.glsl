--Vertex--
#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

--Fragment--
#version 330 core

out vec4 FragColor;

const float near = 0.1f;
const float far = 100.0f;

float LinearizeDepth(float depth);

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0);
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0f -1.0f;
    float linearDepth = (2.0f * near * far) * (far + near - z * (far - near));
    return linearDepth;
}