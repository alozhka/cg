#pragma once

#include "../core/ObjMeshLoader.hpp"
#include "../core/Scene.hpp"
#include "../objects/RayModel.hpp"
#include "../raster_objects/GpuMesh.hpp"

#include <graphics/CompositeDrawable3D.hpp>

#include <glm/ext/matrix_transform.hpp>

#include <memory>

class TeapotScene
{
public:
	static void Build(Scene& scene)
	{
		auto loaded = ObjMeshLoader::Load("assets/models/teapot.obj");
		scene.Add(std::make_unique<RayModel>(loaded.mesh, std::move(loaded.materials), TEAPOT_TRANSFORM));
	}

	static void Build(CompositeDrawable3D& scene)
	{
		auto loaded = ObjMeshLoader::Load("assets/models/teapot.obj");
		scene.AddChild(std::make_shared<GpuMesh>(loaded.mesh, loaded.materials[0], TEAPOT_POSITION));
	}

private:
	static constexpr glm::vec3 TEAPOT_POSITION{ 3.2, 0, -0.2 };
	static constexpr glm::mat4 TEAPOT_TRANSFORM = glm::translate(glm::mat4{ 1 }, TEAPOT_POSITION);
};