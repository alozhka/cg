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

class MeshObject : public ISceneObject
{
public:
	MeshObject(
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
		// Луч в локальную систему меша. Origin и origin+direction
		// преобразуем как точки — корректно при неединичном масштабе.
		const glm::vec3 oLocal = glm::vec3(m_invTransform * glm::vec4(ray.origin, 1.f));
		const glm::vec3 eLocal = glm::vec3(m_invTransform * glm::vec4(ray.origin + ray.direction, 1.f));
		const glm::vec3 dLocal = eLocal - oLocal;
		const float dLen = glm::length(dLocal);
		if (dLen <= 0.f)
		{
			return false;
		}
		const Ray localRay{ oLocal, dLocal / dLen };

		// Текущий hit.t в мировых единицах -> локальные через масштаб направления.
		const float localTMax = hit.t * dLen;

		MeshHit mh;
		if (!m_mesh->Intersect(localRay, localTMax, mh))
		{
			return false;
		}

		// Возврат в мировые единицы.
		const float worldT = mh.t / dLen;
		if (worldT >= hit.t)
		{
			return false;
		}

		glm::vec3 nWorld = glm::normalize(
			m_normalMatrix * m_mesh->InterpolatedNormal(mh.triangleIndex, mh.u, mh.v));
		if (glm::dot(nWorld, ray.direction) > 0.f)
		{
			nWorld = -nWorld;
		}

		hit.t = worldT;
		hit.point = ray.origin + ray.direction * worldT;
		hit.normal = nWorld;
		hit.material = m_materials[m_mesh->Triangles()[mh.triangleIndex].materialIndex];
		return true;
	}

private:
	std::shared_ptr<const RayMesh> m_mesh;
	std::vector<MaterialPtr> m_materials;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;
	glm::mat3 m_normalMatrix;
};