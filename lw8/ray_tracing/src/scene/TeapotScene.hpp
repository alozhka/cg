#pragma once

#include "../core/Scene.hpp"
#include "../objects/MeshObject.hpp"
#include "../objects/ObjMeshLoader.hpp"
#include "../objects/Plane.hpp"
#include "../raster_objects/GpuMesh.hpp"
#include "../shading/Light.hpp"
#include "../shading/MaterialData.hpp"

#include <graphics/CompositeDrawable3D.hpp>

#include <glm/ext/matrix_transform.hpp>

#include <memory>

class TeapotScene
{
public:
	static void Build(Scene& scene)
	{
		auto loaded = LoadTeapot();
		scene.Add(std::make_unique<MeshObject>(loaded.mesh, std::move(loaded.materials), TEAPOT_TRANSFORM));
	}

	static void Build(CompositeDrawable3D& scene)
	{
		auto loaded = LoadTeapot();
		scene.AddChild(std::make_shared<GpuMesh>(loaded.mesh, GreenMaterial(), TEAPOT_POSITION));
	}

private:
	inline static const glm::vec3 TEAPOT_POSITION{ 3.2f, 0.f, -0.2f };
	inline static const glm::mat4 TEAPOT_TRANSFORM = glm::translate(glm::mat4{ 1 }, TEAPOT_POSITION);

	static MaterialPtr GreenMaterial()
	{
		static const MaterialPtr material = std::make_shared<MaterialData>(MaterialData{
			.ambient = glm::vec3{ 0.05, 0.1, 0.05 },
			.diffuse = glm::vec3{ 0.2, 0.7, 0.3 },
			.specular = glm::vec3{ 0.6 },
			.shininess = 64,
		});
		return material;
	}

	static ObjMeshLoader::Loaded LoadTeapot()
	{
		auto loaded = ObjMeshLoader::Load("assets/models/teapot.obj");
		// Чайник в файле без MTL, поэтому материалов всего один (default).
		// Заменяем его на наш зелёный.
		for (auto& m : loaded.materials)
		{
			m = GreenMaterial();
		}
		return loaded;
	}
};