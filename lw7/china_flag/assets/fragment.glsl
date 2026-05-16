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

const vec2 rightFoldNormal = vec2(0.809016994, -0.587785252); // (cos 72, -sin 72)
const vec2 leftFoldNormal = vec2(-0.809016994, -0.587785252);

vec2 reflectOutsideHalfPlane(vec2 point, vec2 normal)
{
    return point - 2.0 * max(dot(normal, point), 0.0) * normal;
}

float cross2d(vec2 a, vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

vec2 foldToReferenceStarArm(vec2 point)
{
    point.x = abs(point.x);
    point = reflectOutsideHalfPlane(point, rightFoldNormal);
    point = reflectOutsideHalfPlane(point, leftFoldNormal);
    point.x = abs(point.x);

    return point;
}

float signedDistanceToStar(vec2 point, float outerRadius)
{
    const float innerToOuterRadiusRatio = 0.381966011;

    point = foldToReferenceStarArm(point);
    point.y -= outerRadius;

    vec2 starArmEdge = innerToOuterRadiusRatio * vec2(-rightFoldNormal.y, rightFoldNormal.x) - vec2(0.0, 1.0);
    float edgeProjectionFactor = clamp(dot(point, starArmEdge) / dot(starArmEdge, starArmEdge), 0.0, outerRadius);
    vec2 closestPointOnEdge = starArmEdge * edgeProjectionFactor;
    float distanceToEdge = length(point - closestPointOnEdge);
    float side = sign(cross2d(starArmEdge, point));

    return distanceToEdge * side;
}

bool insideStar(vec2 p, Star star)
{
    vec2 d = p - star.center;
    float c = cos(-star.rotation);
    float s = sin(-star.rotation);
    vec2 q = vec2(c * d.x - s * d.y, s * d.x + c * d.y);
    q.y = -q.y;

    return signedDistanceToStar(q, star.radius) <= 0.0;
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
