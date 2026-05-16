#version 330 core

in vec2 vWorldPos;

struct Star
{
    vec2 center;
    float radius;
    float rotation;
};

uniform Star uStars[5];

out vec4 FragColor;

// Signed distance to a regular 5-point star. The tip points along local +Y.
float sdStar5(vec2 p, float r, float rf)
{
    const vec2 k1 = vec2(0.809016994, -0.587785252); // (cos 72, -sin 72)
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

bool insideStar(vec2 p, Star star)
{
    vec2 d = p - star.center;
    float c = cos(-star.rotation);
    float s = sin(-star.rotation);
    vec2 q = vec2(c * d.x - s * d.y, s * d.x + c * d.y);
    q.y = -q.y;

    const float kStarRatio = 0.381966011;
    return sdStar5(q, star.radius, kStarRatio) <= 0.0;
}

void main()
{
    const vec3 flagColor = vec3(0.93, 0.11, 0.15);
    const vec3 starColor = vec3(1.0, 1.0, 0.0);

    bool isStar = false;
    for (int i = 0; i < 5; ++i)
    {
        if (insideStar(vWorldPos, uStars[i]))
        {
            isStar = true;
            break;
        }
    }

    vec3 color = isStar ? starColor : flagColor;
    FragColor = vec4(color, 1.0);
}
