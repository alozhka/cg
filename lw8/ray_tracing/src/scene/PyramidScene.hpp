#pragma once

#include "Plane.hpp"
#include "Scene.hpp"
#include "Torus.hpp"
#include "../shading/Light.hpp"
#include "../shading/MaterialData.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <memory>

class PyramidScene
{
public:
	static void Build(Scene& scene)
	{
		const Materials mats = BuildMaterials();
		AddFloor(scene, mats.floor);
		AddPyramid(scene, mats);
		AddLights(scene);
	}

private:
	static constexpr float TUBE_RADIUS = 0.12f;

	struct Materials
	{
		MaterialPtr floor;
		MaterialPtr red;
		MaterialPtr orange;
		MaterialPtr yellow;
		MaterialPtr green;
		MaterialPtr blue;
	};

	struct TorusSpec
	{
		float R;
		float y;
		MaterialPtr material;
	};

	static Materials BuildMaterials()
	{
		Materials m;
		m.floor = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.2 },
			.diffuse = glm::vec3{ 0.6 },
			.specular = glm::vec3{ 0.05 },
			.shininess = 8,
		});
		m.red = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.2, 0.04, 0.04 },
			.diffuse = glm::vec3{ 0.9, 0.2, 0.2 },
			.specular = glm::vec3{ 0.3 },
			.shininess = 16,
		});
		m.orange = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.2, 0.1, 0.02 },
			.diffuse = glm::vec3{ 0.95, 0.55, 0.15 },
			.specular = glm::vec3{ 0.4 },
			.shininess = 32,
		});
		m.yellow = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.2, 0.18, 0.04 },
			.diffuse = glm::vec3{ 0.95, 0.9, 0.2 },
			.specular = glm::vec3{ 0.4 },
			.shininess = 64,
		});
		m.green = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.04, 0.2, 0.04 },
			.diffuse = glm::vec3{ 0.2, 0.9, 0.2 },
			.specular = glm::vec3{ 0.6 },
			.shininess = 64,
		});
		m.blue = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.04, 0.08, 0.2 },
			.diffuse = glm::vec3{ 0.2, 0.4, 0.9 },
			.specular = glm::vec3{ 0.9 },
			.shininess = 256,
		});
		return m;
	}

	static void AddFloor(Scene& scene, const MaterialPtr& floor)
	{
		scene.Add(std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, floor));
	}

	static void AddPyramid(Scene& scene, const Materials& mats)
	{
		const TorusSpec specs[] = {
			{ 0.90f, 1 * TUBE_RADIUS, mats.red },
			{ 0.75f, 3 * TUBE_RADIUS, mats.orange },
			{ 0.60f, 5 * TUBE_RADIUS, mats.yellow },
			{ 0.45f, 7 * TUBE_RADIUS, mats.green },
			{ 0.30f, 9 * TUBE_RADIUS, mats.blue },
		};
		for (const auto& s : specs)
		{
			const glm::mat4 transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 0.f, s.y, 0.f });
			scene.Add(std::make_unique<Torus>(s.R, TUBE_RADIUS, transform, s.material));
		}
	}

	static void AddLights(Scene& scene)
	{
		scene.Add(std::make_unique<DirectLight>(
			glm::vec3{ -1, -1, -0.4 },
			glm::vec3{ 0.15 },
			glm::vec3{ 0.7 },
			glm::vec3{ 0.8 }));
		scene.Add(std::make_unique<PointLight>(
			glm::vec3{ 1.5, 1.8, 1.5 },
			glm::vec3{ 0 },
			glm::vec3{ 0.5, 0.4, 0.3 },
			glm::vec3{ 0.5, 0.4, 0.3 }));
	}
};