#version 330 core

layout (location = 0) in vec2 aUV;

uniform mat4 uMVP;
uniform float uPhase;
uniform float uR;
uniform float uMobiusHalfWidth;

out vec3 vNormal;

const float PI = 3.1415926;
const float TWO_PI = 6.2831853;

vec3 GetMobiusPos(float u, float t)
{
    float v = mix(-uMobiusHalfWidth, uMobiusHalfWidth, t);
    float r = uR + v * cos(u * 0.5);
    return vec3(r * cos(u), r * sin(u), v * sin(u * 0.5));
}

vec3 GetKleinPos(float u, float t)
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

    return vec3(x, y, z) * 0.08;
}

vec3 GetSurface(float u, float t)
{
    return mix(GetMobiusPos(u, t), GetKleinPos(u, t), uPhase);
}

vec3 GetSurfaceNormal(float u, float t)
{
    const float EPS = 0.001;
    vec3 du = GetSurface(u + EPS, t) - GetSurface(u - EPS, t);
    vec3 dt = GetSurface(u, t + EPS) - GetSurface(u, t - EPS);
    return normalize(cross(du, dt));
}

void main()
{
    float u = aUV.x * TWO_PI;
    float t = aUV.y;

    vec3 p = GetSurface(u, t);
    vNormal = GetSurfaceNormal(u, t);

    gl_Position = uMVP * vec4(p, 1.0);
}