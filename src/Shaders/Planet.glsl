--Vertex--
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm;

out vec3 fragPos;
out vec3 transNorm;
out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));
    transNorm = aNorm * norm;
    texCoord = aTexCoord;
}

--Fragment--
#version 330 core

in vec3 fragPos;
in vec2 texCoord;
in vec3 transNorm;
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;

vec3 ambient = 0.1f * objColor;

float rand(float x)
{
    return fract(sin(x) * 100000.0f);
}

float rand2D(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 1.0);
}

float rand3D(vec3 st)
{
    return fract(sin(dot(st.xyz, vec3(12.9898, 78.233, 32.7933))) * 1.0);
}

float noise(float x)
{
    float i = floor(x);
    float f = fract(x);
    return mix(rand(i), rand(i + 1), smoothstep(0.0, 1.0, f));
}

float noise2D(vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);

    // rand values at corners of 2D tile.
    float a = rand2D(i);
    float b = rand2D(i + vec2(1.0, 0.0));
    float c = rand2D(i + vec2(0.0, 1.0));
    float d = rand2D(i + vec2(1.0, 1.0));

    // Smooth interpolation.
    vec2 u = smoothstep(0.0, 1.0, f);

    // Mix corner percentages.
    return  mix(a, b, u.x) +
            (c-a) * u.y * (1.0-u.x) +
            (d-b) * u.x * u.y;
}

float noise3D(vec3 st)
{
    vec3 i = floor(st);
    vec3 f = fract(st);

    // 8 corners of the 3D cube
    float a = rand3D(i);
    float b = rand3D(i + vec3(1.0, 0.0, 0.0));
    float c = rand3D(i + vec3(0.0, 1.0, 0.0));
    float d = rand3D(i + vec3(1.0, 1.0, 0.0));
    float e = rand3D(i + vec3(0.0, 0.0, 1.0));
    float f1 = rand3D(i + vec3(1.0, 0.0, 1.0));
    float g = rand3D(i + vec3(0.0, 1.0, 1.0));
    float h = rand3D(i + vec3(1.0, 1.0, 1.0));

    vec3 u = smoothstep(0.0, 1.0, f);

    // Interpolate along x for each face.
    float x00 = mix(a, b, u.x);
    float x10 = mix(c, d, u.x);
    float x01 = mix(e, f1, u.x);
    float x11 = mix(g, h, u.x);

    // Interpolate along y for the lower and upper z planes.
    float y0 = mix(x00, x10, u.y);
    float y1 = mix(x01, x11, u.y);

    // Interpolate along z.
    return mix(y0, y1, u.z);
}

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

float gNoise2D(vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);
    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, transNorm);

    float diff = max(dot(transNorm, lightDir), 0.0f);
    float spec = max(dot(viewDir, reflectDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor * 0;  // removed for now;

    vec3 light = ambient + diffuse + specular;
    
    // Attempting noise pattern using fragment position.
    // vec3 iPos = floor(fragPos * 10);
    // vec2 iPos = floor(texCoord * 1);
    // vec3 randCol = vec3(rand3D(iPos));
    // float val = noise3D(iPos * 10.0);
    // FragColor = vec4(randCol, 1.0);

    // Value noise attempts.
    // vec2 randCol = vec2(noise2D(texCoord * 50));
    // vec3 randCol = vec3(noise3D(vec3(texCoord, 1.0) * 10));

    // Gradient noise first attempt.
    // vec2 randCol = vec2(gNoise2D(texCoord * 25));

    vec2 newCoord;
    newCoord.y = texCoord.y * 1;
    newCoord.x = texCoord.x * 0.2;
    vec3 randCol = vec3(gNoise2D(newCoord * 25));

    vec3 col1 = vec3(0.2, 0.0, 0.4);
    vec3 col2 = vec3(0.4, 0.0, 1.0);
    vec3 newCol = mix(col1, col2, randCol);

    FragColor = vec4(newCol * light, 1.0);
}