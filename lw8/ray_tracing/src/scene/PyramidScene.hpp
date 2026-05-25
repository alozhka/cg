#pragma once

#include "../core/Scene.hpp"
#include "../objects/Plane.hpp"
#include "../objects/Torus.hpp"
#include "../raster_objects/GpuPlane.hpp"
#include "../raster_objects/GpuTorus.hpp"
#include "../shading/Light.hpp"
#include "../shading/MaterialData.hpp"
#include "graphics/CompositeDrawable3D.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <memory>

class PyramidScene
{
public:
	static void Build(Scene& scene)
	{
		AddFloor(scene);
		AddPyramid(scene);
		AddLights(scene);
	}

	static void Build(CompositeDrawable3D& scene)
	{
		AddFloor(scene);
		AddPyramid(scene);
	}

private:
	static constexpr float TUBE_RADIUS = 0.12;

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

	static void AddFloor(Scene& scene)
	{
		scene.Add(std::make_shared<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, MATERIALS.floor));
	}

	static void AddFloor(CompositeDrawable3D& scene)
	{
		auto plane = std::make_shared<GpuPlane>(MATERIALS.floor);
		scene.AddChild(plane);
	}

	static void AddPyramid(Scene& scene)
	{
		const std::array<TorusSpec, 5> specs = BuildPyramidSpecs();
		for (const auto& s : specs)
		{
			const glm::mat4 transform = glm::translate(glm::mat4{ 1 }, glm::vec3{ 0, s.y, 0 });
			scene.Add(std::make_shared<Torus>(s.R, TUBE_RADIUS, transform, s.material));
		}
	}

	static void AddPyramid(CompositeDrawable3D& scene)
	{
		const std::array<TorusSpec, 5> specs = BuildPyramidSpecs();
		for (const auto& s : specs)
		{
			scene.AddChild(std::make_shared<GpuTorus>(s.R, TUBE_RADIUS, glm::vec3{ 0, s.y, 0 }, s.material));
		}
	}

	static void AddLights(Scene& scene)
	{
		std::vector<std::shared_ptr<ILight>> lights = BuildLights();
		for (const auto& light : lights)
		{
			scene.Add(light);
		}
	}

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

	static std::vector<std::shared_ptr<ILight>> BuildLights()
	{
		std::vector<std::shared_ptr<ILight>> lights;
		lights.push_back(std::make_shared<DirectLight>(
			glm::vec3{ -1, -1, -0.4 },
			glm::vec3{ 0.15 },
			glm::vec3{ 0.7 },
			glm::vec3{ 0.8 }));
		lights.push_back(std::make_shared<PointLight>(
			glm::vec3{ 1.5, 1.8, 1.5 },
			glm::vec3{ 0 },
			glm::vec3{ 0.5, 0.4, 0.3 },
			glm::vec3{ 0.5, 0.4, 0.3 }));
		return lights;
	}

	static std::array<TorusSpec, 5> BuildPyramidSpecs()
	{
		const std::array<TorusSpec, 5> specs = {
			{
				{ 0.90, 1 * TUBE_RADIUS, MATERIALS.red },
				{ 0.75, 3 * TUBE_RADIUS, MATERIALS.orange },
				{ 0.60, 5 * TUBE_RADIUS, MATERIALS.yellow },
				{ 0.45, 7 * TUBE_RADIUS, MATERIALS.green },
				{ 0.30, 9 * TUBE_RADIUS, MATERIALS.blue },
			}
		};
		return specs;
	}

	inline static Materials MATERIALS = BuildMaterials();
};