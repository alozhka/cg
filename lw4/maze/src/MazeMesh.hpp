#pragma once

#include <array>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

#include <memory>
#include <vector>

class MazeMesh
{
public:
	MazeMesh()
		: m_floorMesh(BuildFloor())
		, m_ceilingMesh(BuildCeiling())
		, m_wallsMesh(BuildWalls())
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		DrawFloor(shader, viewProjection);
		DrawCeiling(shader, viewProjection);
		DrawWalls(shader, viewProjection);
	}

	static bool IsWall(float x, float z)
	{
		int col = static_cast<int>(std::floor(x));
		int row = static_cast<int>(std::floor(z));

		if (IsOutOfMaze(row, col))
		{
			return true;
		}

		return MAZE_GRID[row][col] == 1;
	}

private:
	void DrawFloor(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformVec4("uColor", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		m_floorMesh.Draw(GL_TRIANGLES);
	}

	void DrawCeiling(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformVec4("uColor", glm::vec4(0.6f, 0.6f, 0.65f, 1.0f));
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		m_ceilingMesh.Draw(GL_TRIANGLES);
	}

	void DrawWalls(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformVec4("uColor", glm::vec4(0.7f, 0.4f, 0.2f, 1.0f));
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		m_wallsMesh.Draw(GL_TRIANGLES);
	}

	static std::vector<Vertex> BuildFloor()
	{
		std::vector<Vertex> vertices;

		AddQuad(
			vertices,
			{ 0, 0, 0 },
			{ MAZE_CELLS, 0, 0 },
			{ MAZE_CELLS, 0, MAZE_CELLS },
			{ 0, 0, MAZE_CELLS },
			{ 0, 1, 0 });

		return vertices;
	}

	static std::vector<Vertex> BuildCeiling()
	{
		std::vector<Vertex> vertices;

		AddQuad(
			vertices,
			{ 0, WALL_HEIGHT, MAZE_CELLS },
			{ MAZE_CELLS, WALL_HEIGHT, MAZE_CELLS },
			{ MAZE_CELLS, WALL_HEIGHT, 0 },
			{ 0, WALL_HEIGHT, 0 },
			{ 0, -1, 0 });

		return vertices;
	}

	static std::vector<Vertex> BuildWalls()
	{
		std::vector<Vertex> vertices;

		for (int row = 0; row < MAZE_CELLS; ++row)
		{
			for (int col = 0; col < MAZE_CELLS; ++col)
			{
				if (IsOpenBlock(row, col))
				{
					continue;
				}

				TryAddSouthWall(vertices, row, col);
				TryAddNorthWall(vertices, row, col);
				TryAddEastWall(vertices, row, col);
				TryAddWestWall(vertices, row, col);
			}
		}

		return vertices;
	}

	static void TryAddSouthWall(std::vector<Vertex>& vertices, int row, int col)
	{
		if (IsOpenBlock(row + 1, col))
		{
			AddQuad(vertices,
				{ col, 0, row + 1 },
				{ col + 1, 0, row + 1 },
				{ col + 1, WALL_HEIGHT, row + 1 },
				{ col, WALL_HEIGHT, row + 1 },
				{ 0, 0, 1 });
		}
	}

	static void TryAddNorthWall(std::vector<Vertex>& vertices, int row, int col)
	{
		if (IsOpenBlock(row - 1, col))
		{
			AddQuad(vertices,
				{ col + 1, 0, row },
				{ col, 0, row },
				{ col, WALL_HEIGHT, row },
				{ col + 1, WALL_HEIGHT, row },
				{ 0, 0, -1 });
		}
	}

	static void TryAddEastWall(std::vector<Vertex>& vertices, int row, int col)
	{
		if (IsOpenBlock(row, col + 1))
		{
			AddQuad(vertices,
				{ col + 1, 0, row + 1 },
				{ col + 1, 0, row },
				{ col + 1, WALL_HEIGHT, row },
				{ col + 1, WALL_HEIGHT, row + 1 },
				{ 1, 0, 0 });
		}
	}

	static void TryAddWestWall(std::vector<Vertex>& vertices, int row, int col)
	{
		if (IsOpenBlock(row, col - 1))
		{
			AddQuad(vertices,
				{ col, 0, row },
				{ col, 0, row + 1 },
				{ col, WALL_HEIGHT, row + 1 },
				{ col, WALL_HEIGHT, row },
				{ -1, 0, 0 });
		}
	}

	static void AddQuad(std::vector<Vertex>& vertices,
		glm::vec3 a,
		glm::vec3 b,
		glm::vec3 c,
		glm::vec3 d,
		glm::vec3 normal)
	{
		vertices.push_back({ a, normal });
		vertices.push_back({ b, normal });
		vertices.push_back({ c, normal });
		vertices.push_back({ a, normal });
		vertices.push_back({ c, normal });
		vertices.push_back({ d, normal });
	}

	static bool IsOpenBlock(int row, int col)
	{
		if (IsOutOfMaze(row, col))
		{
			return false;
		}

		return MAZE_GRID[row][col] == 0;
	}

	static bool IsOutOfMaze(int row, int col)
	{
		return row < 0 || row >= MAZE_CELLS || col < 0 || col >= MAZE_CELLS;
	}

	static constexpr float WALL_HEIGHT = 1;
	static constexpr int MAZE_CELLS = 16;
	static constexpr std::array<std::array<int, MAZE_CELLS>, MAZE_CELLS> MAZE_GRID = {
		{
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
		}
	};

	Mesh m_floorMesh;
	Mesh m_ceilingMesh;
	Mesh m_wallsMesh;
};