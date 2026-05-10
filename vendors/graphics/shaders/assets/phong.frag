#version 330 core

#define MAX_POINT_LIGHTS 4

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    int hasDiffuseTexture;
    sampler2D diffuseTexture;
};

struct DirectLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material uMaterial;
uniform DirectLight uDirectLight;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform int uPointLightCount;
uniform vec3 uCameraPos;

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 baseDiffuse)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * uMaterial.ambient * baseDiffuse;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * uMaterial.diffuse * baseDiffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = light.specular * spec * uMaterial.specular;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 baseDiffuse)
{
    vec3 lightDir = normalize(light.position - vFragPos);

    vec3 ambient = light.ambient * uMaterial.ambient * baseDiffuse;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * uMaterial.diffuse * baseDiffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = light.specular * spec * uMaterial.specular;

    float distance = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPos - vFragPos);

    vec3 baseDiffuse = vec3(1.0);
    if (uMaterial.hasDiffuseTexture == 1)
    {
        baseDiffuse = texture(uMaterial.diffuseTexture, vTexCoord).rgb;
    }

    vec3 result = CalcDirectLight(uDirectLight, normal, viewDir, baseDiffuse);

    int count = min(uPointLightCount, MAX_POINT_LIGHTS);
    for (int i = 0; i < count; ++i)
    {
        result += CalcPointLight(uPointLights[i], normal, viewDir, baseDiffuse);
    }

    FragColor = vec4(result, 1.0);
}