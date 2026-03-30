#version 330 core

in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform vec4 uColor;
uniform vec3 uCameraPos;

struct DirectLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectLight uDirectLight;

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uDirectLight.direction);
    vec3 viewDirection = normalize(uCameraPos - vFragPos);

    // Invert normal if it looks away from the camera
    if (dot(norm, viewDirection) < 0.0)
    {
        norm = -norm;
    }

    // Ambient
    vec3 ambient = uDirectLight.ambient * uColor.rgb;

    // Diffuse
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = uDirectLight.diffuse * diff * uColor.rgb;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    vec3 specular = uDirectLight.specular * spec;

    FragColor = vec4(ambient + diffuse + specular, uColor.a);
}