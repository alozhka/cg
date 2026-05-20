#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform vec2 uOrigin;
uniform float uTime;
out vec4 FragColor;

const float FRONT_SPEED   = 0.65;
const float RING_DENSITY  = 35.0;
const float RING_FALLOFF  = 5.5;
const float WARP_STRENGTH = 0.07;
const float REVEAL_RATE   = 0.1;
const float CREST_SHADOW  = 0.65;

void main()
{
    float dist = distance(vTexCoords, uOrigin);

    float phase = uTime * FRONT_SPEED - dist;

    vec2 displacedUV = vTexCoords;
    float revealBlend = 0;
    float brightness = 1;

    if (phase > 0)
    {
        float envelope = exp(-RING_FALLOFF * phase);
        float ripple = sin(RING_DENSITY * phase);

        vec2 dir = normalize(vTexCoords - uOrigin);
        displacedUV = vTexCoords + dir * (WARP_STRENGTH * envelope * ripple);

        revealBlend = smoothstep(0, REVEAL_RATE, phase);
        brightness = 1 - CREST_SHADOW * envelope * abs(ripple);
    }

    vec4 color = mix(texture(uTex0, displacedUV), texture(uTex1, displacedUV), revealBlend);
    FragColor = vec4(color.rgb * brightness, 1);
}
