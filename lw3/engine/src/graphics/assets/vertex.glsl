#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat3 uViewProjection;

void main()
{
    vec3 pos = uViewProjection * vec3(aPos, 1);
    gl_Position = vec4(pos.x, pos.y, 0, 1);
}