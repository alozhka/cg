#version 330 core

in vec2 vWorldPos;

uniform vec3 uBackground;
uniform vec3 uStarColor;
// xy = центр, z = внешний радиус, w = угол поворота (рад)
uniform vec4 uStars[5];

out vec4 FragColor;

// SDF правильной 5-конечной звезды (Inigo Quilez).
// "Tip" звезды направлен в +Y локального фрейма.
// r — внешний радиус, rf — отношение внутр./внеш.
float sdStar5(vec2 p, float r, float rf)
{
    const vec2 k1 = vec2(0.809016994, -0.587785252); // (cos72°, -sin72°)
    const vec2 k2 = vec2(-0.809016994, -0.587785252);
    p.x = abs(p.x);
    p -= 2.0 * max(dot(k1, p), 0.0) * k1;
    p -= 2.0 * max(dot(k2, p), 0.0) * k2;
    p.x = abs(p.x);
    p.y -= r;
    vec2 ba = rf * vec2(-k1.y, k1.x) - vec2(0.0, 1.0);
    float h = clamp(dot(p, ba) / dot(ba, ba), 0.0, r);
    return length(p - ba * h) * sign(p.y * ba.x - p.x * ba.y);
}

bool insideStar(vec2 p, vec4 star)
{
    vec2 d = p - star.xy;
    float c = cos(-star.w);
    float s = sin(-star.w);
    vec2 q = vec2(c * d.x - s * d.y, s * d.x + c * d.y);
    // В мире y вниз; "tip вверх" в мире = -Y. Внутри SDF tip — это +Y,
    // поэтому ось Y инвертируем перед подачей.
    q.y = -q.y;
    const float kStarRatio = 0.381966011;
    return sdStar5(q, star.z, kStarRatio) <= 0.0;
}

void main()
{
    vec3 color = uBackground;
    for (int i = 0; i < 5; ++i)
    {
        if (insideStar(vWorldPos, uStars[i]))
        {
            color = uStarColor;
        }
    }
    FragColor = vec4(color, 1.0);
}