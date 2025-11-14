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

vec3 ambient = 0.0 * objColor;

const int OCTAVES = 4;

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

    // Gradient noise.
    // vec3 noise = vec3(gNoise2D(newCoord * 25));

    // FBM noise.
    // vec3 noise = vec3(pattern(texCoord * 1));
    vec2 q, r;
    vec3 noise = vec3(colorPattern(texCoord, q, r));

    // vec3 col1 = vec3(0.659, 0.486, 0.537);
    // vec3 col2 = vec3(0.612, 0.643, 0.541);
    // vec3 col3 = vec3(0.686, 0.686, 0.580);
    // vec3 col4 = vec3(0.753, 0.635, 0.608);

    vec3 col1 = vec3(0.3,0.05,0.05);
    vec3 col2 = vec3(0.9,0.9,0.9);
    vec3 col3 = vec3(0.5,0.2,0.2);
    // vec3 col4 = vec3(0.8, 0.7, 0.7);
    vec3 col4 = vec3(0.4);

    vec3 color = vec3(0.2,0.1,0.4);
    color = mix(col1, col2, noise);
    color = mix(color, col3, dot(r,r));
    // color = mix(color, col4, 1.5*r.y*r.y);
    color = mix(color, col4, 0.5*smoothstep(1.2,1.3,abs(r.y)+abs(r.x)));

    // vec3 test = vec3(dot(r, r));
    // vec3 test = vec3(dot(q, q));
    vec3 test = vec3(2.0 * q.y * q.y);

    // color *= (noise * 2.0);
    // color = vec3(1.0) - color;
    // color /= 1.5;
    // color = mix(color, col4, 0.5*smoothstep(1.2,1.3,abs(r.y)+abs(r.x)));

    FragColor = vec4(color * light, 1.0);
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
    float h = 1.0;

    q.x = fbm( st + vec2(0.0,0.0), h);
    q.y = fbm( st + vec2(5.2,1.3), h);

    r.x = fbm( st + 4.0*q + vec2(1.7,9.2), h);
    r.y = fbm( st + 4.0*q + vec2(8.3,2.8), h);

    return fbm( st + 4.0*r, h);
}