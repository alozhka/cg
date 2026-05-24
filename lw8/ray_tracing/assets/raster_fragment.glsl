#version 330 core

out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


uniform Material uMaterial;

void main()
{

    FragColor = vec4(uMaterial.ambient, 1.0);
}