#pragma once

#include "../core/HitInfo.hpp"
#include "../core/Ray.hpp"
#include "../core/Scene.hpp"
#include "PhongShader.hpp"

#include <algorithm>
#include <cstdint>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

class Shading
{
public:
	static glm::vec3 TraceRay(const Scene& scene, const Ray& ray)
	{
		HitInfo hit;
		if (!scene.Intersect(ray, hit))
		{
			return SkyGradient(ray);
		}

		return PhongShader::Shade(hit, -ray.direction, scene);
	}

	static uint32_t PackColor(glm::vec3 c)
	{
		c = glm::clamp(c, glm::vec3{ 0 }, glm::vec3{ 1 });

		auto r = static_cast<uint8_t>(c.r * 255);
		auto g = static_cast<uint8_t>(c.g * 255);
		auto b = static_cast<uint8_t>(c.b * 255);

		constexpr uint8_t a = 0xFF;

		return static_cast<uint32_t>(a) << 24
			| static_cast<uint32_t>(r) << 16
			| static_cast<uint32_t>(g) << 8
			| static_cast<uint32_t>(b);
	}

private:
	static glm::vec3 SkyGradient(const Ray& ray)
	{
		const float t = 0.5f * (ray.direction.y + 1);
		return glm::mix(glm::vec3{ 1 }, glm::vec3{ 0.5, 0.7, 1 }, t);
	}
};