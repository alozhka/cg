#version 330 core

layout (location = 0) in vec2 aUV;

uniform mat4 uMVP;
uniform float uPhase;
uniform float uR;
uniform float uMobiusHalfWidth;

const float PI = 3.14159265358979323846;
const float TWO_PI = 6.28318530717958647692;

vec3 mobiusPos(float u, float t)
{
    float v = mix(-uMobiusHalfWidth, uMobiusHalfWidth, t);
    float r = uR + v * cos(u * 0.5);
    return vec3(r * cos(u), r * sin(u), v * sin(u * 0.5));
}

vec3 kleinPos(float u, float t)
{
    float v = t * TWO_PI;
    float cu = cos(u);
    float su = sin(u);
    float cv = cos(v);
    float sv = sin(v);
    float r = 4 * (1 - 0.5 * cu);

    float x;
    float y;
    if (u < PI)
    {
        x = 6 * cu * (1 + su) + r * cu * cv;
        y = 16 * su + r * su * cv;
    }
    else
    {
        x = 6 * cu * (1 + su) - r * cv;
        y = 16 * su;
    }
    float z = r * sv;

    // Сжимаем к ~единичному масштабу и центрируем по вертикали
    return vec3(x, y, z) * 0.08;
}

void main()
{
    float u = aUV.x * TWO_PI;
    float t = aUV.y;

    vec3 p = mix(mobiusPos(u, t), kleinPos(u, t), uPhase);
    gl_Position = uMVP * vec4(p, 1.0);
}
