#pragma once

#include "HitInfo.hpp"
#include "Ray.hpp"

class ISceneObject
{
public:
	virtual ~ISceneObject() = default;

	// Если луч пересекает объект с t > eps и ближе текущего hit.t,
	// обновляет поля hit и возвращает true.
	virtual bool Intersect(const Ray& ray, HitInfo& hit) const = 0;
};