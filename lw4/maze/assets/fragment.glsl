#version 330 core

#define MAX_POINT_LIGHTS 4

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

uniform DirectLight uDirectLight;
uniform int uNumPointLights;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];

vec3 CalcLight(vec3 lightDir, vec3 ambient, vec3 diffuse, vec3 specular, vec3 norm, vec3 viewDirection)
{
    vec3 ambientResult = ambient * uColor.rgb;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseResult = diffuse * diff * uColor.rgb;

    vec3 reflectDirection = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    vec3 specularResult = specular * spec;

    return ambientResult + diffuseResult + specularResult;
}

vec3 CalcDirectLight(DirectLight light, vec3 norm, vec3 viewDirection)
{
    return CalcLight(normalize(light.direction), light.ambient, light.diffuse, light.specular, norm, viewDirection);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDirection)
{
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 lightDir = normalize(light.position - fragPos);
    return CalcLight(lightDir, light.ambient, light.diffuse, light.specular, norm, viewDirection) * attenuation;
}

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 viewDirection = normalize(uCameraPos - vFragPos);

    vec3 result = CalcDirectLight(uDirectLight, norm, viewDirection);

    for (int i = 0; i < uNumPointLights; i++)
    {
        result += CalcPointLight(uPointLights[i], norm, vFragPos, viewDirection);
    }

    FragColor = vec4(result, uColor.a);
}