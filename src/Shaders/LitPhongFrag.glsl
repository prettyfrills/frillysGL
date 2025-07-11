#version 330 core

struct Material
{
    // For fine control over different lighting colors.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float roughness;
};

struct Light
{
    // For fine control over different lighting intensities.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 fragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material matr;
uniform Light lght;

vec3 ambient = matr.ambient * lght.ambient;
vec3 norm = normalize(Normal);

void main()
{
    // Get light direction, find alignment of normals, add color.
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lght.diffuse * (diff * matr.diffuse);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightPos, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matr.roughness);
    vec3 specular = lght.specular * (spec * matr.specular);

    vec3 light = ambient + diffuse + specular;
    FragColor = vec4(light, 1.0);
}