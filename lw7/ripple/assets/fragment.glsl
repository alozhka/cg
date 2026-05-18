#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform vec2 uOrigin;
uniform float uTime;
out vec4 FragColor;

const float WAVE_SPEED = 0.65;
const float FREQUENCY = 22.0;
const float DECAY = 5.5;
const float AMPLITUDE = 0.07;
const float BLEND_RANGE = 0.4;
const float BRIGHTNESS_K = 0.65;

void main()
{
    float d = distance(vTexCoords, uOrigin);

    float phase = uTime * WAVE_SPEED - d;

    vec2 sampleUV = vTexCoords;
    float blend = 0.0;
    float brightness = 1.0;

    if (phase > 0.0)
    {
        float envelope = exp(-DECAY * phase);
        float wave = sin(FREQUENCY * phase);

        vec2 dir = normalize(vTexCoords - uOrigin );
        sampleUV = vTexCoords + dir * (AMPLITUDE * envelope * wave);

        blend = smoothstep(0.0, BLEND_RANGE, phase);
        brightness = 1.0 - BRIGHTNESS_K * envelope * abs(wave);
    }

    vec4 color = mix(texture(uTex0, sampleUV), texture(uTex1, sampleUV), blend);
    FragColor = vec4(color.rgb * brightness, 1.0);
}
