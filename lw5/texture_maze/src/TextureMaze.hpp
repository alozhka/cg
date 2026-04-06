#pragma once

#include "MazeGrid.hpp"
#include "TextureMazeMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>

class TextureMaze
{
public:
	explicit TextureMaze(glm::vec3 playerPos = { 1.5, 0.5, 1.5 })
		: m_playerPos(playerPos)
	{
	}

	void UpdateMovement(glm::vec3 direction, float dt)
	{
		glm::vec3 moved = glm::normalize(direction) * MOVE_SPEED * dt;
		m_playerPos = ResolveMovement(m_playerPos, moved);
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		m_mazeMesh.Draw(shader, viewProjection);
	}

	glm::vec3 GetPlayerPosition() const
	{
		return m_playerPos;
	}

private:
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

		glm::vec3 tryZ = pos + glm::vec3(0, 0, move.z);
		if (!IsBlocked(tryZ))
		{
			return tryZ;
		}

		return pos;
	}

	static bool IsBlocked(const glm::vec3& p)
	{
		return MAZE_GRID.IsWall(p.x + PLAYER_HALF_SIZE, p.z + PLAYER_HALF_SIZE)
			|| MAZE_GRID.IsWall(p.x - PLAYER_HALF_SIZE, p.z + PLAYER_HALF_SIZE)
			|| MAZE_GRID.IsWall(p.x + PLAYER_HALF_SIZE, p.z - PLAYER_HALF_SIZE)
			|| MAZE_GRID.IsWall(p.x - PLAYER_HALF_SIZE, p.z - PLAYER_HALF_SIZE);
	}

	static constexpr float MOVE_SPEED = 3;
	static constexpr float PLAYER_HALF_SIZE = 0.2;
	static constexpr float WALL_HEIGHT = 1;
	static constexpr int MAZE_CELLS = 16;
	static constexpr MazeGrid<MAZE_CELLS> MAZE_GRID{ { {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	} } };

	TextureProvider m_textures{
		{ "floor", "assets/textures/floor.jpg" },
		{ "ceiling", "assets/textures/ceiling.jpg" },
		{ "wall_0", "assets/textures/brick.jpg" },
		{ "wall_1", "assets/textures/stone.jpg" },
		{ "wall_2", "assets/textures/wood.jpg" },
		{ "wall_3", "assets/textures/concrete.jpg" },
		{ "wall_4", "assets/textures/tile.jpg" },
		{ "wall_5", "assets/textures/golden_freddie.jpg" },
	};

	glm::vec3 m_playerPos;
	TextureMazeMesh<MAZE_CELLS> m_mazeMesh{ MAZE_GRID, WALL_HEIGHT, m_textures };
};