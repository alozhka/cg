#pragma once

#include "../core/ObjMeshLoader.hpp"
#include "../core/Scene.hpp"
#include "../objects/MeshObject.hpp"
#include "../raster_objects/GpuMesh.hpp"
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
		scene.AddChild(std::make_shared<GpuMesh>(loaded.mesh, CreateGreenMaterial(), TEAPOT_POSITION));
	}

private:
	static constexpr glm::vec3 TEAPOT_POSITION{ 3.2, 0, -0.2 };
	static constexpr glm::mat4 TEAPOT_TRANSFORM = glm::translate(glm::mat4{ 1 }, TEAPOT_POSITION);

	static MaterialPtr CreateGreenMaterial()
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
		for (auto& m : loaded.materials)
		{
			m = CreateGreenMaterial();
		}
		return loaded;
	}
};