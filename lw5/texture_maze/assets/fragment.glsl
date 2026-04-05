#version 330 core

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
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
    vec3 baseColor = texture(uTexture, vTexCoord).rgb;

    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uDirectLight.direction);
    vec3 viewDirection = normalize(uCameraPos - vFragPos);

    // Ambient
    vec3 ambient = uDirectLight.ambient * baseColor;

    // Diffuse
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = uDirectLight.diffuse * diff * baseColor;

    // Specular (Phong)
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    vec3 specular = uDirectLight.specular * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}