#version 330 core

in vec2 vUv;
out vec4 fragColor;

uniform vec3 uCameraPos;
uniform mat4 uInvViewProjection;

struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct Hit
{
	float t;
	vec3 point;
	vec3 normal;
};

const float MAX_T = 1e30;
const float EPSILON = 1e-4;

const vec3 FLOOR_AMBIENT = vec3(0.2);
const vec3 FLOOR_DIFFUSE = vec3(0.6);
const vec3 FLOOR_SPECULAR = vec3(0.05);
const float FLOOR_SHININESS = 8.0;

const vec3 LIGHT_TO_LIGHT = normalize(-vec3(-1.0, -1.0, -0.4));
const vec3 LIGHT_AMBIENT = vec3(0.15);
const vec3 LIGHT_DIFFUSE = vec3(0.7);
const vec3 LIGHT_SPECULAR = vec3(0.8);

Ray PrimaryRay(vec2 uv)
{
	float u = 2.0 * uv.x - 1.0;
	float v = 1.0 - 2.0 * uv.y;

	vec4 farClip = vec4(u, v, 1.0, 1.0);
	vec4 farWorld = uInvViewProjection * farClip;
	farWorld /= farWorld.w;

	vec3 dir = normalize(farWorld.xyz - uCameraPos);
	return Ray(uCameraPos, dir);
}

bool IntersectPlane(Ray ray, vec3 point, vec3 normal, inout Hit hit)
{
	float denom = dot(normal, ray.dir);
	if (abs(denom) < EPSILON)
	{
		return false;
	}

	float t = dot(point - ray.origin, normal) / denom;
	if (t < EPSILON || t >= hit.t)
	{
		return false;
	}

	hit.t = t;
	hit.point = ray.origin + ray.dir * t;
	hit.normal = denom < 0.0 ? normal : -normal;
	return true;
}

vec3 SkyGradient(Ray ray)
{
	float t = 0.5 * (ray.dir.y + 1.0);
	return mix(vec3(1.0), vec3(0.5, 0.7, 1.0), t);
}

vec3 Shade(Hit hit, vec3 dirToObserver)
{
	vec3 color = FLOOR_AMBIENT * LIGHT_AMBIENT;

	float ndl = dot(hit.normal, LIGHT_TO_LIGHT);
	if (ndl > 0.0)
	{
		color += FLOOR_DIFFUSE * LIGHT_DIFFUSE * ndl;

		vec3 reflected = reflect(-LIGHT_TO_LIGHT, hit.normal);
		float rdv = max(0.0, dot(reflected, dirToObserver));
		if (rdv > 0.0)
		{
			color += FLOOR_SPECULAR * LIGHT_SPECULAR * pow(rdv, FLOOR_SHININESS);
		}
	}

	return color;
}

void main()
{
	Ray ray = PrimaryRay(vUv);

	Hit hit;
	hit.t = MAX_T;

	if (IntersectPlane(ray, vec3(0.0), vec3(0.0, 1.0, 0.0), hit))
	{
		fragColor = vec4(Shade(hit, -ray.dir), 1.0);
	}
	else
	{
		fragColor = vec4(SkyGradient(ray), 1.0);
	}
}
