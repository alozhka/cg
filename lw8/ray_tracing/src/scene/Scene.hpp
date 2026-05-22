#pragma once

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

private:
	std::vector<std::unique_ptr<ISceneObject>> m_objects;
};