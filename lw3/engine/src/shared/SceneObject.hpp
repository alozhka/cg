#pragma once

#include "Point.hpp"

#include <GLFW/glfw3.h>
#include <memory>

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

class SceneObject
{
public:
	explicit SceneObject()
		: m_pos({ 0, 0 })
		, m_rotation(0)
		, m_scale(1)
	{
	}

	SceneObject(const Point& m_pos, double m_rotation, double m_scale)
		: m_pos(m_pos)
		, m_rotation(m_rotation)
		, m_scale(m_scale)
	{
	}

	virtual ~SceneObject() = default;

	void SetPosition(double x, double y)
	{
		m_pos = { x, y };
	}

	void SetRotation(double angleInDegrees)
	{
		m_rotation = angleInDegrees;
	}

	void SetScale(double scale)
	{
		m_scale = scale;
	}

	virtual void Update(double dt)
	{
	}

	virtual void Draw() = 0;

protected:
	void ApplyTransform()
	{
		glTranslated(m_pos.x, m_pos.y, 0);
		glRotated(m_rotation, 0, 0, 1);
		glScaled(m_scale, m_scale, 1);
	}

	Point m_pos;
	double m_rotation;
	double m_scale;
};