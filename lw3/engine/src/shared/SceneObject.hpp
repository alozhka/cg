#pragma once

#include "Point.hpp"
#include "../render/ShaderProgram.hpp"
#include "../render/Mat3.hpp"

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

	virtual void Draw(ShaderProgram& shader, const Mat3& parentTransform) = 0;

protected:
	Mat3 GetTransformMatrix() const
	{
		return Mat3::Translate(m_pos.x, m_pos.y)
			* Mat3::Rotate(m_rotation)
			* Mat3::Scale(m_scale, m_scale);
	}

	Point m_pos;
	double m_rotation;
	double m_scale;
};