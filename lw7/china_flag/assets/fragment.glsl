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

const float PI = 3.1415926;
const float INNER_TO_OUTER_RADIUS_RATIO = 0.381966011;

bool IsPointOnTheLeft(vec2 p0, vec2 p1, vec2 p)
{
    vec2 edge = p1 - p0;
    vec2 normal = vec2(-edge.y, edge.x);
    return dot(p - p0, normal) > -1e-6;
}

bool IsPointInsideTriangle(vec2 p0, vec2 p1, vec2 p2, vec2 p)
{
    return
        IsPointOnTheLeft(p0, p1, p) &&
        IsPointOnTheLeft(p1, p2, p) &&
        IsPointOnTheLeft(p2, p0, p);
}

vec2[10] BuildStarVertices(float outerRadius, float rotation)
{
    float innerRadius = outerRadius * INNER_TO_OUTER_RADIUS_RATIO;
    vec2 vertices[10];
    for (int i = 0; i < 10; ++i)
    {
        float angle = PI / 2.0 - rotation + PI / 5.0 * float(i);
        float r = (i % 2 == 0) ? outerRadius : innerRadius;
        vertices[i] = vec2(cos(angle), sin(angle)) * r;
    }
    return vertices;
}

bool IsInsideStar(vec2 worldPoint, Star star)
{
    vec2 p = worldPoint - star.center;
    p.y = -p.y;

    vec2 vertices[10] = BuildStarVertices(star.radius, star.rotation);

    for (int i = 0; i < 10; ++i)
    {
        vec2 p1 = vertices[i];
        vec2 p2 = vertices[(i + 1) % 10];
        if (IsPointInsideTriangle(vec2(0.0), p1, p2, p))
        {
            return true;
        }
    }

    return false;
}

void main()
{
    const vec3 flagColor = vec3(0.93, 0.11, 0.15);
    const vec3 starColor = vec3(1.0, 1.0, 0.0);

    bool isStar = false;
    for (int i = 0; i < 5; ++i)
    {
        if (IsInsideStar(vWorldPos, uStars[i]))
        {
            isStar = true;
            break;
        }
    }

    vec3 color = isStar ? starColor : flagColor;
    FragColor = vec4(color, 1.0);
}
