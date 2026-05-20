#version 330 core

layout (location = 0) in float aX;

uniform mat4 uMVP;

void main()
{
    float x = aX;
    // R=(1+sinx)(1+0,9cos8x)(1+0,1cos24x)(0,5+0,05cos140x)
    float R = (1 + sin(x)) // кардиойда
        * (1 + 0.9 * cos(8 * x)) // крупные лепестки
        * (1 + 0.1 * cos(24 * x)) // изменение формы лепестка (субколебания)
        * (0.5 + 0.05 * cos(140 * x)); // шероховатость на лепестке (высокочастотные колебания)

    // x'=R×cos(x)
    // y'=R×sin(x)
    vec3 p = vec3(R * cos(x), R * sin(x), 0);
    gl_Position = uMVP * vec4(p, 1);
}