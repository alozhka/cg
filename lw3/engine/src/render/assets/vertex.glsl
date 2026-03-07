#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat3 uTransform;
uniform mat3 uProjection;

void main()
{
    vec3 pos = uProjection * uTransform * vec3(aPos, 1);
    gl_Position = vec4(pos.x, pos.y, 0, 1);
}