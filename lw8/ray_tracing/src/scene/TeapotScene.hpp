#pragma once

#include "MeshObject.hpp"
#include "ObjMeshLoader.hpp"
#include "Plane.hpp"
#include "Scene.hpp"
#include "../shading/Light.hpp"
#include "../shading/MaterialData.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <memory>

class TeapotScene
{
public:
	static void Build(Scene& scene)
	{
		AddFloor(scene);
		AddTeapot(scene);
		AddLights(scene);
	}

private:
	static void AddFloor(Scene& scene)
	{
		auto floor = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.2 },
			.diffuse = glm::vec3{ 0.6 },
			.specular = glm::vec3{ 0.05 },
			.shininess = 8,
		});
		scene.Add(std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, floor));
	}

	static void AddTeapot(Scene& scene)
	{
		auto loaded = ObjMeshLoader::Load("assets/models/teapot.obj");

		// Teapot из Blender ~3 единицы в высоту с центром около y=0.
		// Опустим его так, чтобы дно стояло на полу.
		auto teapotMat = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.05, 0.1, 0.05 },
			.diffuse = glm::vec3{ 0.2, 0.7, 0.3 },
			.specular = glm::vec3{ 0.6 },
			.shininess = 64,
		});
		// Заменяем дефолтный материал чайника на наш зелёный.
		// Чайник в файле без MTL, поэтому материалов всего один (default).
		for (auto& m : loaded.materials)
		{
			m = teapotMat;
		}

		const glm::mat4 transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 0.f, 0.f, 0.f });
		scene.Add(std::make_unique<MeshObject>(loaded.mesh, std::move(loaded.materials), transform));
	}

	static void AddLights(Scene& scene)
	{
		scene.Add(std::make_unique<DirectLight>(
			glm::vec3{ -1, -1, -0.4 },
			glm::vec3{ 0.15 },
			glm::vec3{ 0.7 },
			glm::vec3{ 0.8 }));
		scene.Add(std::make_unique<PointLight>(
			glm::vec3{ 2.0, 3.0, 2.0 },
			glm::vec3{ 0 },
			glm::vec3{ 0.5, 0.4, 0.3 },
			glm::vec3{ 0.5, 0.4, 0.3 }));
	}
};