#pragma once

#include "../shading/Light.hpp"
#include "ISceneObject.hpp"

#include <memory>
#include <utility>
#include <vector>

class Scene
{
public:
	void Add(std::unique_ptr<ISceneObject> object)
	{
		m_objects.push_back(std::move(object));
	}

	void Add(std::unique_ptr<ILight> light)
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

	const std::vector<std::unique_ptr<ILight>>& GetLights() const
	{
		return m_lights;
	}

private:
	std::vector<std::unique_ptr<ISceneObject>> m_objects;
	std::vector<std::unique_ptr<ILight>> m_lights;
};