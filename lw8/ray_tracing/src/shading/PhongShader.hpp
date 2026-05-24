#pragma once

#include "../core/HitInfo.hpp"
#include "../core/Ray.hpp"
#include "../core/Scene.hpp"
#include "Light.hpp"
#include "MaterialData.hpp"

#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

class PhongShader
{
public:
	static glm::vec3 Shade(
		const HitInfo& hit,
		const glm::vec3& dirToObserver,
		const Scene& scene)
	{
		const MaterialData& m = *hit.material;
		glm::vec3 color{ 0 };

		for (auto& light : scene.GetLights())
		{
			LightSample s = light->Sample(hit.point);

			color += m.ambient * s.ambient;

			Ray shadowRay{ hit.point + hit.normal * SHADOW_BIAS, s.direction };
			if (scene.IsOverlappedByObject(shadowRay, s.distance - SHADOW_BIAS))
			{
				continue;
			}

			float ndl = glm::dot(hit.normal, s.direction);
			if (ndl > 0)
			{
				color += m.diffuse * s.diffuse * ndl;

				glm::vec3 reflected = glm::reflect(-s.direction, hit.normal);
				float rdv = std::max(0.f, glm::dot(reflected, dirToObserver));
				if (rdv > 0)
				{
					color += m.specular * s.specular * std::pow(rdv, m.shininess);
				}
			}
		}

		return color;
	}

private:
	static constexpr float SHADOW_BIAS = 1e-3f;
};