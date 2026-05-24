#pragma once

#include "HitInfo.hpp"
#include "Ray.hpp"

class ISceneObject
{
public:
	virtual ~ISceneObject() = default;

	virtual bool Intersect(const Ray& ray, HitInfo& hit) const = 0;
};