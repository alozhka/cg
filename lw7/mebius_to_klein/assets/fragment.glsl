#version 330 core

in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 uColor;
uniform vec3 uLightDir;
uniform vec3 uCameraPos;

out vec4 FragColor;

const float AMBIENT = 0.15;

void main()
{
    vec3 n = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPos - vFragPos);

    if (dot(n, viewDir) < 0)
    {
        n = -n;
    }

    float diff = max(0, dot(n, uLightDir));
    FragColor = vec4(uColor * (AMBIENT + (1 - AMBIENT) * diff), 1);
}