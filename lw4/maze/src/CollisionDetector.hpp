#pragma once

#include "Maze.hpp"

#include <glm/glm.hpp>

class CollisionDetector
{
public:
	static glm::vec3 ResolveMovement(const glm::vec3& pos, const glm::vec3& move)
	{
		glm::vec3 newPos = pos + move;

		if (!IsBlocked(newPos))
		{
			return newPos;
		}

		glm::vec3 tryX = pos + glm::vec3(move.x, 0, 0);
		if (!IsBlocked(tryX))
		{
			return tryX;
		}

		glm::vec3 tryZ = pos + glm::vec3(0.f, 0.f, move.z);
		if (!IsBlocked(tryZ))
		{
			return tryZ;
		}

		return pos;
	}

private:
	static bool IsBlocked(const glm::vec3& p)
	{
		return Maze::IsWall(p.x + MARGIN, p.z + MARGIN)
			|| Maze::IsWall(p.x - MARGIN, p.z + MARGIN)
			|| Maze::IsWall(p.x + MARGIN, p.z - MARGIN)
			|| Maze::IsWall(p.x - MARGIN, p.z - MARGIN);
	}

	static constexpr float MARGIN = 0.2f;
};