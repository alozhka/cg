#pragma once

#include <array>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Maze
{
public:
	Maze()
	{
		std::vector<Vertex> vertices;

		BuildFloor(vertices);
		m_floorCount = vertices.size();

		BuildCeiling(vertices);
		m_ceilingCount = vertices.size() - m_floorCount;

		m_wallStart = vertices.size();
		BuildWalls(vertices);
		m_wallCount = vertices.size() - m_wallStart;

		m_mesh = std::make_unique<Mesh>(vertices);
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		DrawFloor(shader, viewProjection);
		DrawCeiling(shader, viewProjection);
		DrawWalls(shader, viewProjection);
	}

private:
	void DrawFloor(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformVec4("uColor", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		m_mesh->Draw(GL_TRIANGLES, 0, m_floorCount);
	}

	void DrawCeiling(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformVec4("uColor", glm::vec4(0.6f, 0.6f, 0.65f, 1.0f));
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		m_mesh->Draw(GL_TRIANGLES, m_floorCount, m_ceilingCount);
	}

	void DrawWalls(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformVec4("uColor", glm::vec4(0.7f, 0.4f, 0.2f, 1.0f));
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		m_mesh->Draw(GL_TRIANGLES, m_wallStart, m_wallCount);
	}

	static constexpr float WALL_HEIGHT = 1.0f;

	static void AddQuad(std::vector<Vertex>& vertices,
		glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
		glm::vec3 normal)
	{
		vertices.push_back({ a, normal });
		vertices.push_back({ b, normal });
		vertices.push_back({ c, normal });
		vertices.push_back({ a, normal });
		vertices.push_back({ c, normal });
		vertices.push_back({ d, normal });
	}

	static void BuildFloor(std::vector<Vertex>& vertices)
	{
		AddQuad(
			vertices,
			{ 0, 0, 0 },
			{ MAZE_GRID_SIZE, 0, 0 },
			{ MAZE_GRID_SIZE, 0, MAZE_GRID_SIZE },
			{ 0, 0, MAZE_GRID_SIZE },
			{ 0, 1, 0 });
	}

	static void BuildCeiling(std::vector<Vertex>& vertices)
	{
		AddQuad(
			vertices,
			{ 0, WALL_HEIGHT, MAZE_GRID_SIZE },
			{ MAZE_GRID_SIZE, WALL_HEIGHT, MAZE_GRID_SIZE },
			{ MAZE_GRID_SIZE, WALL_HEIGHT, 0 },
			{ 0, WALL_HEIGHT, 0 },
			{ 0, -1, 0 });
	}

	static void TryAddSouthWall(std::vector<Vertex>& vertices, int row, int col, float x, float z)
	{
		if (row + 1 < MAZE_GRID_SIZE && MAZE_GRID[row + 1][col] == 0)
		{
			AddQuad(vertices,
				{ x, 0, z + 1 }, { x + 1, 0, z + 1 }, { x + 1, WALL_HEIGHT, z + 1 }, { x, WALL_HEIGHT, z + 1 },
				{ 0, 0, 1 });
		}
	}

	static void TryAddNorthWall(std::vector<Vertex>& vertices, int row, int col, float x, float z)
	{
		if (row - 1 >= 0 && MAZE_GRID[row - 1][col] == 0)
		{
			AddQuad(vertices,
				{ x + 1, 0, z }, { x, 0, z }, { x, WALL_HEIGHT, z }, { x + 1, WALL_HEIGHT, z },
				{ 0, 0, -1 });
		}
	}

	static void TryAddEastWall(std::vector<Vertex>& vertices, int row, int col, float x, float z)
	{
		if (col + 1 < MAZE_GRID_SIZE && MAZE_GRID[row][col + 1] == 0)
		{
			AddQuad(vertices,
				{ x + 1, 0, z + 1 }, { x + 1, 0, z }, { x + 1, WALL_HEIGHT, z }, { x + 1, WALL_HEIGHT, z + 1 },
				{ 1, 0, 0 });
		}
	}

	static void TryAddWestWall(std::vector<Vertex>& vertices, int row, int col, float x, float z)
	{
		if (col - 1 >= 0 && MAZE_GRID[row][col - 1] == 0)
		{
			AddQuad(vertices,
				{ x, 0, z }, { x, 0, z + 1 }, { x, WALL_HEIGHT, z + 1 }, { x, WALL_HEIGHT, z },
				{ -1, 0, 0 });
		}
	}

	static void BuildWalls(std::vector<Vertex>& vertices)
	{
		for (int row = 0; row < MAZE_GRID_SIZE; ++row)
		{
			for (int col = 0; col < MAZE_GRID_SIZE; ++col)
			{
				if (MAZE_GRID[row][col] == 0)
				{
					continue;
				}

				float x = static_cast<float>(col);
				float z = static_cast<float>(row);

				TryAddSouthWall(vertices, row, col, x, z);
				TryAddNorthWall(vertices, row, col, x, z);
				TryAddEastWall(vertices, row, col, x, z);
				TryAddWestWall(vertices, row, col, x, z);
			}
		}
	}

	static constexpr int MAZE_CELLS = 16;
	static constexpr int MAZE_GRID_SIZE = MAZE_CELLS * 2 + 1;

	static constexpr std::array<std::array<int, MAZE_GRID_SIZE>, MAZE_GRID_SIZE> MAZE_GRID = { {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	} };

	std::unique_ptr<Mesh> m_mesh;
	size_t m_floorCount = 0;
	size_t m_ceilingCount = 0;
	size_t m_wallStart = 0;
	size_t m_wallCount = 0;
};