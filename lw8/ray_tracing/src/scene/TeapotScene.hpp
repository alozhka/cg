#pragma once

#include "../core/Scene.hpp"
#include "../objects/MeshObject.hpp"
#include "../objects/ObjMeshLoader.hpp"
#include "../objects/Plane.hpp"
#include "../shading/Light.hpp"
#include "../shading/MaterialData.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <memory>

class TeapotScene
{
public:
	static void Build(Scene& scene)
	{
		AddTeapot(scene);
	}

private:
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

		const glm::mat4 transform = glm::translate(glm::mat4{ 1 }, glm::vec3{ 3.2, 0, -0.2 });
		scene.Add(std::make_unique<MeshObject>(loaded.mesh, std::move(loaded.materials), transform));
	}
};