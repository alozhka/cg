#version 330 core

in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform vec4 uColor;
uniform vec3 uCameraPos;

struct DirectLight
{
    vec3 direction;  // Направление ОТ источника (как в твоем коде)
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectLight uDirectLight;

void main()
{
    vec3 norm = normalize(vNormal);
    // Важно: uDirectLight.direction обычно направлена ОТ света, поэтому для расчётов инвертируем
    vec3 lightDir = normalize(-uDirectLight.direction);
    vec3 viewDir = normalize(uCameraPos - vFragPos);

    // Если нормаль смотрит ОТ камеры (dot < 0), инвертируем её.
    // Это гарантирует, что "видимая" сторона всегда будет считаться лицевой для освещения.
    if (dot(norm, viewDir) < 0.0)
    {
        norm = -norm;
    }

    // --- Ambient ---
    vec3 ambient = uDirectLight.ambient * uColor.rgb;

    // --- Diffuse ---
    // Теперь dot будет положительным для видимой стороны, если на неё падает свет
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uDirectLight.diffuse * diff * uColor.rgb;

    // --- Specular (Phong) ---
    // Отражение считаем от скорректированной нормали
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = uDirectLight.specular * spec;

    FragColor = vec4(ambient + diffuse + specular, uColor.a);
}