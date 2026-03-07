#version 330 core

layout (location = 0) vec2 in aPos;

uniform mat4 uTransform;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uTransform * vec3(aPos, 0);
}