#pragma once

#include "../shading/Light.hpp"
#include "ISceneObject.hpp"

#include <memory>
#include <utility>
#include <vector>

class Scene
{
public:
	void Add(const std::shared_ptr<ISceneObject>& object)
	{
		m_objects.push_back(object);
	}

	void Add(std::shared_ptr<ILight> light)
	{
		m_lights.push_back(std::move(light));
	}

	bool Intersect(const Ray& ray, HitInfo& hit) const
	{
		bool anyHit = false;
		for (const auto& object : m_objects)
		{
			if (object->Intersect(ray, hit))
			{
				anyHit = true;
			}
		}
		return anyHit;
	}

	bool IsOverlappedByObject(const Ray& ray, float maxDist) const
	{
		for (const auto& object : m_objects)
		{
			HitInfo probe;
			probe.t = maxDist;
			if (object->Intersect(ray, probe))
			{
				return true;
			}
		}
		return false;
	}

	const std::vector<std::shared_ptr<ILight>>& GetLights() const
	{
		return m_lights;
	}

private:
	std::vector<std::shared_ptr<ISceneObject>> m_objects;
	std::vector<std::shared_ptr<ILight>> m_lights;
};