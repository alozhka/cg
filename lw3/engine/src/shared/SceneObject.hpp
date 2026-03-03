#pragma once

#include "Point.hpp"
#include "Mat3.hpp"

#include <memory>

class ShaderProgram;

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

	SceneObject(const Point& pos, double rotation, double scale)
		: m_pos(pos)
		, m_rotation(rotation)
		, m_scale(scale)
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

	virtual void Draw(ShaderProgram& shader, const Mat3& parentModel) = 0;

	Mat3 GetModelMatrix() const
	{
		Mat3 t = Mat3::Translation(static_cast<float>(m_pos.x), static_cast<float>(m_pos.y));
		Mat3 r = Mat3::RotationDegrees(static_cast<float>(m_rotation));
		Mat3 s = Mat3::Scale(static_cast<float>(m_scale), static_cast<float>(m_scale));
		return t * r * s;
	}

protected:
	Point m_pos;
	double m_rotation;
	double m_scale;
};
