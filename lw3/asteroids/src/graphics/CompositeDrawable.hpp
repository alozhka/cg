#pragma once
#include "Drawable.hpp"

#include <vector>

class CompositeDrawable : public Drawable
{
public:
	void AddChild(const SceneObjectPtr& child)
	{
		m_children.push_back(child);
	}

	void Update(float dt) override
	{
		for (const SceneObjectPtr& child : m_children)
		{
			child->Update(dt);
		}
	}

	void Draw(ShaderProgram& shader, const Mat3& parentTransform) override
	{
		Mat3 worldTransform = parentTransform * GetTransformMatrix();

		for (const SceneObjectPtr& child : m_children)
		{
			child->Draw(shader, worldTransform);
		}
	}

private:
	std::vector<SceneObjectPtr> m_children;
};