#pragma once

#include "Mat3.hpp"
#include "ShaderProgram.hpp"
#include "Vec2f.hpp"

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

	SceneObject(const Vec2f& pos, float rotation, float scale)
		: m_pos(pos)
		, m_rotation(rotation)
		, m_scale(scale)
	{
	}

	virtual ~SceneObject() = default;

	void SetPosition(float x, float y)
	{
		m_pos = { x, y };
	}

	void SetRotation(float angleInDegrees)
	{
		m_rotation = angleInDegrees;
	}

	void SetScale(float scale)
	{
		m_scale = scale;
	}

	virtual void Update(float dt)
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

	Vec2f m_pos;
	float m_rotation;
	float m_scale;
};