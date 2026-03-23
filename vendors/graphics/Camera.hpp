#pragma once
#include "Mat4.hpp"
#include "Vec3.hpp"

class Camera
{
public:
	Camera(const Vec3& position, const Vec3& target, const Vec3& up)
		: m_position(position)
		, m_target(target)
		, m_up(up)
	{
	}

	Mat4 GetViewMatrix() const
	{
		return Mat4::LookAt(m_position, m_target, m_up);
	}

private:
	Vec3 m_position;
	Vec3 m_target;
	Vec3 m_up;
};