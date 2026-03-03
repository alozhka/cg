#pragma once
#include "SceneObject.hpp"
#include "../render/ShaderProgram.hpp"

#include <vector>

class CompositeObject : public SceneObject
{
public:
	void AddChild(const SceneObjectPtr& child)
	{
		m_children.push_back(child);
	}

	void Update(double dt) override
	{
		for (const SceneObjectPtr& child : m_children)
		{
			child->Update(dt);
		}
	}

	void Draw(ShaderProgram& shader, const Mat3& parentModel) override
	{
		Mat3 currentModel = parentModel * GetModelMatrix();

		for (const SceneObjectPtr& child : m_children)
		{
			child->Draw(shader, currentModel);
		}
	}

private:
	std::vector<SceneObjectPtr> m_children;
};
