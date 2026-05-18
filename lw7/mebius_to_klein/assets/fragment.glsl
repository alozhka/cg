#version 330 core

in vec3 vNormal;

uniform vec3 uColor;
uniform vec3 uLightDir;

out vec4 FragColor;

const float AMBIENT = 0.15;

void main()
{
    vec3 n = normalize(vNormal);
    if (!gl_FrontFacing) n = -n;

    float diff = max(0.0, dot(n, uLightDir));
    FragColor = vec4(uColor * (AMBIENT + (1.0 - AMBIENT) * diff), 1.0);
}