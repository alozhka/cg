#pragma once

#include "shaders/ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Drawable3D;
using SceneObject3DPtr = std::shared_ptr<Drawable3D>;

class Drawable3D
{
public:
	Drawable3D() = default;

	Drawable3D(
		const glm::vec3& pos,
		const glm::vec3& rotationDegrees = glm::vec3(0),
		const glm::vec3& scale = glm::vec3(1))
		: m_pos(pos)
		, m_rotation(rotationDegrees)
		, m_scale(scale)
	{
	}

	virtual ~Drawable3D() = default;

	void SetPosition(const glm::vec3& pos) { m_pos = pos; }
	void SetRotation(const glm::vec3& rotationDegrees) { m_rotation = rotationDegrees; }
	void SetScale(const glm::vec3& scale) { m_scale = scale; }
	void SetScale(float scale) { m_scale = glm::vec3(scale); }

	const glm::vec3& GetPosition() const { return m_pos; }
	const glm::vec3& GetRotation() const { return m_rotation; }
	const glm::vec3& GetScale() const { return m_scale; }

	virtual void Update(float dt)
	{
	}

	virtual void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) = 0;

protected:
	glm::mat4 GetTransformMatrix() const
	{
		glm::mat4 transform(1);
		transform = glm::translate(transform, m_pos);
		transform = glm::rotate(transform, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, m_scale);
		return transform;
	}

	glm::vec3 m_pos{ 0, 0, 0 };
	glm::vec3 m_rotation{ 0, 0, 0 };
	glm::vec3 m_scale{ 1, 1, 1 };
};
