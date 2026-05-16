#version 330 core

// 2D-флаг: aPos приходит в мировых координатах квада;
// uMVP переводит их в clip space, фрагмент рисует звёзды по vWorldPos.

layout (location = 0) in vec2 aPos;

uniform mat4 uMVP;

out vec2 vWorldPos;

void main()
{
    vec4 worldPos = vec4(aPos, 0.0, 1.0);

    vWorldPos = aPos;
    gl_Position = uMVP * worldPos;
}