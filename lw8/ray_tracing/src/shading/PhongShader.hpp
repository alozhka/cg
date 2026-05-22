#pragma once

#include "../core/HitInfo.hpp"
#include "Light.hpp"
#include "MaterialData.hpp"

#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

class PhongShader
{
public:
	// viewDir — единичный вектор ОТ точки К наблюдателю.
	static glm::vec3 Shade(
		const HitInfo& hit,
		const glm::vec3& viewDir,
		const std::vector<std::unique_ptr<ILight>>& lights)
	{
		const MaterialData& m = *hit.material;
		glm::vec3 color{ 0 };

		for (const auto& light : lights)
		{
			const LightSample s = light->Sample(hit.point);

			color += m.ambient * s.ambient;

			const float ndl = glm::dot(hit.normal, s.direction);
			if (ndl > 0)
			{
				color += m.diffuse * s.diffuse * ndl;

				// R = 2(N·L)N - L; specular = (R·V)^shininess.
				const glm::vec3 reflected = 2.f * ndl * hit.normal - s.direction;
				const float rdv = std::max(0.f, glm::dot(reflected, viewDir));
				if (rdv > 0)
				{
					color += m.specular * s.specular * std::pow(rdv, m.shininess);
				}
			}
		}

		return color;
	}
};