#pragma once

#include "../core/ISceneObject.hpp"
#include "../core/RayMesh.hpp"
#include "../shading/MaterialData.hpp"

#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <utility>
#include <vector>

class RayModel : public ISceneObject
{
public:
	RayModel(
		std::shared_ptr<const RayMesh> mesh,
		std::vector<MaterialPtr> materials,
		const glm::mat4& transform = glm::mat4{ 1.f })
		: m_mesh(std::move(mesh))
		, m_materials(std::move(materials))
		, m_transform(transform)
		, m_invTransform(glm::inverse(transform))
		, m_normalMatrix(glm::transpose(glm::mat3(m_invTransform)))
	{
	}

	bool Intersect(const Ray& ray, HitInfo& hit) const override
	{
		Ray localRay = CreateLocalRay(ray);

		MeshHit meshHit;
		if (!m_mesh->Intersect(localRay, hit.t, meshHit))
		{
			return false;
		}

		if (meshHit.t >= hit.t)
		{
			return false;
		}

		FillHit(ray, meshHit, hit);
		return true;
	}

private:
	Ray CreateLocalRay(const Ray& worldRay) const
	{
		glm::vec3 origin = glm::vec3(m_invTransform * glm::vec4(worldRay.origin, 1.f));
		glm::vec3 direction = glm::mat3(m_invTransform) * worldRay.direction;
		return Ray{ origin, direction };
	}

	void FillHit(const Ray& worldRay, const MeshHit& meshHit, HitInfo& hit) const
	{
		glm::vec3 meshNormal = m_mesh->InterpolatedNormal(meshHit.triangleIndex, meshHit.u, meshHit.v);
		glm::vec3 normal = glm::normalize(m_normalMatrix * meshNormal);
		if (glm::dot(normal, worldRay.direction) > 0)
		{
			normal = -normal;
		}

		hit.t = meshHit.t;
		hit.point = worldRay.At(meshHit.t);
		hit.normal = normal;
		hit.material = MaterialAt(meshHit.triangleIndex);
	}

	MaterialPtr MaterialAt(uint32_t triangleIndex) const
	{
		return m_materials[m_mesh->Triangles()[triangleIndex].materialIndex];
	}

	std::shared_ptr<const RayMesh> m_mesh;
	std::vector<MaterialPtr> m_materials;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;
	glm::mat3 m_normalMatrix;
};