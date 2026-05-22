#pragma once

#include <graphics/camera/FirstPersonCamera.hpp>

#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

struct Ray
{
	glm::vec3 origin{ 0.f };
	glm::vec3 direction{ 0.f, 0.f, -1.f };

	// Первичный луч через точку (u, v) на плоскости проекции,
	// где u, v ∈ [-1, 1], v растёт вверх. fov — вертикальный угол обзора (рад),
	// aspect = width / height.
	static Ray Primary(const FirstPersonCamera& camera,
		float u, float v, float fov, float aspect)
	{
		const float halfH = std::tan(fov * 0.5f);
		const float halfW = halfH * aspect;

		const glm::vec3 dir = glm::normalize(
			camera.GetForward()
			+ camera.GetRight() * (u * halfW)
			+ camera.GetUp() * (v * halfH));

		return Ray{ camera.GetPosition(), dir };
	}
};