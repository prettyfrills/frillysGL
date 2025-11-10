--Vertex--
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm;

out vec2 texCoord;
out vec3 transNorm;
out vec3 fragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    transNorm = aNorm * norm;
}

--Fragment--
#version 330 core

in vec2 texCoord;
in vec3 transNorm;
in vec3 fragPos;
out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

vec3 ambient = 0.1 * objColor;

const int OCTAVES = 2;

vec2 random2(vec2 st);
float gNoise2D(vec2 st);
float fbm(vec2 st, float h);
float pattern(vec2 st);
float colorPattern(vec2 st, out vec2 q, out vec2 r);

void main()
{
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, transNorm);

    float diff = max(dot(transNorm, lightDir), 0.0);
    float spec = max(dot(viewDir, reflectDir), 0.0);

    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor * 0;  // Add back in after using noise as a specular mask.
    vec3 light = ambient + diffuse + specular;

    vec2 newCoord;
    newCoord.y = texCoord.y * 1;
    newCoord.x = texCoord.x * 0.2;
    newCoord += pattern(newCoord);

    // vec3 noise = vec3(gNoise2D(newCoord * 25));
    vec3 noise = vec3(pattern(texCoord * 1));

    vec3 col1 = vec3(0.659, 0.486, 0.537);
    vec3 col2 = vec3(0.612, 0.643, 0.541);
    vec3 newCol = mix(col1, col2, noise);

    FragColor = vec4(newCol, 1.0);
}

vec2 random2(vec2 st)
{
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

float fbm(vec2 st, float h)     // h determines the memory, and thus, visual qualities of the fbm curve.
{
    float G = exp2(-h);
    float f = 1.0;
    float a = 1.0;
    float t = 0.0;

    for(int i = 0; i < OCTAVES; i++)
    {
        t += a * gNoise2D(f * st);
        f *= 2.0;
        a *= G;
    }

    return t;
}

float pattern(vec2 st)
{
    //return fbm(st, 0.5);      no domain warping.
    // vec2 q = vec2(fbm(st + vec2(0.0), 0.5), fbm(st + vec2(5.2, 1.3), 0.5));
    // return fbm(st + 4 * q, 0.5);

    vec2 q = vec2( fbm( st + vec2(0.0,0.0), 0.5),
                   fbm( st + vec2(5.2,1.3), 0.5));

    vec2 r = vec2( fbm( st + 4.0*q + vec2(1.7,9.2), 0.5 ),
                   fbm( st + 4.0*q + vec2(8.3,2.8), 0.5 ));

    return fbm(st + (4.0 * r), 0.5);
}

float colorPattern(vec2 st, out vec2 q, out vec2 r)
{
    q.x = fbm( st + vec2(0.0,0.0), 0.5 );
    q.y = fbm( st + vec2(5.2,1.3), 0.5 );

    r.x = fbm( st + 4.0*q + vec2(1.7,9.2), 0.5 );
    r.y = fbm( st + 4.0*q + vec2(8.3,2.8), 0.5 );

    return fbm( st + 4.0*r, 0.5 );
}