#pragma once

#include "../core/HitInfo.hpp"
#include "../core/Ray.hpp"
#include "../scene/Scene.hpp"
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

		for (const auto& light : scene.GetLights())
		{
			const LightSample s = light->Sample(hit.point);

			// Ambient накладывается всегда — даже в тени.
			color += m.ambient * s.ambient;

			// Сдвиг origin вдоль нормали лечит self-shadow acne.
			const Ray shadowRay{ hit.point + hit.normal * SHADOW_BIAS, s.direction };
			if (scene.IsOccluded(shadowRay, s.distance - SHADOW_BIAS))
			{
				continue;
			}

			const float ndl = glm::dot(hit.normal, s.direction);
			if (ndl > 0)
			{
				color += m.diffuse * s.diffuse * ndl;

				// R = 2(N·L)N - L; specular = (R·V)^shininess.
				const glm::vec3 reflected = 2.f * ndl * hit.normal - s.direction;
				const float rdv = std::max(0.f, glm::dot(reflected, dirToObserver));
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