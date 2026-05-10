#pragma once

#include "Drawable3D.hpp"

#include <vector>

class CompositeDrawable3D : public Drawable3D
{
public:
	void AddChild(const SceneObject3DPtr& child)
	{
		m_children.push_back(child);
	}

	void Update(float dt) override
	{
		for (const SceneObject3DPtr& child : m_children)
		{
			child->Update(dt);
		}
	}

	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		glm::mat4 worldTransform = parentTransform * GetTransformMatrix();
		for (const SceneObject3DPtr& child : m_children)
		{
			child->Draw(shader, worldTransform);
		}
	}

private:
	std::vector<SceneObject3DPtr> m_children;
};