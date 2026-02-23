#pragma once
#include "SceneObject.hpp"

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

	void Draw() override
	{
		glPushMatrix();
		ApplyTransform();

		for (const SceneObjectPtr& child : m_children)
		{
			child->Draw();
		}

		glPopMatrix();
	}

private:
	std::vector<SceneObjectPtr> m_children;
};