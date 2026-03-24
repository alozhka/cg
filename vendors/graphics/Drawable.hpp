#pragma once

#include "shaders/ShaderProgram.hpp"

#include <memory>

class Drawable;
using SceneObjectPtr = std::shared_ptr<Drawable>;

class Drawable
{
public:
	explicit Drawable()
		: m_pos({ 0, 0 })
		, m_rotation(0)
		, m_scale(1)
	{
	}

	Drawable(const glm::vec2& pos, float rotation = 0, float scale = 1)
		: m_pos(pos)
		, m_rotation(rotation)
		, m_scale(scale)
	{
	}

	virtual ~Drawable() = default;

	void SetPosition(float x, float y)
	{
		m_pos = { x, y };
	}

	void SetPosition(glm::vec2 pos)
	{
		m_pos = pos;
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

	virtual void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) = 0;

protected:
	glm::mat4 GetTransformMatrix() const
	{
		glm::mat4 transform(1);

		transform = glm::translate(transform, glm::vec3(m_pos, 0));
		transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(m_scale, m_scale, 1));

		return transform;
	}

	glm::vec2 m_pos;
	float m_rotation;
	float m_scale;
};