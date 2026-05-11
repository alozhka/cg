#pragma once
#include <glm/vec3.hpp>

class BoundingBox
{
public:
	BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos)
		: m_minPos(minPos)
		, m_maxPos(maxPos)
	{
	}

private:
	glm::vec3 m_minPos, m_maxPos;
};