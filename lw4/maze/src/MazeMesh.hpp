#pragma once

#include "MazeGrid.hpp"

#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

#include <vector>

template<int N>
class MazeMesh
{
public:
	MazeMesh(const MazeGrid<N>& grid, float wallHeight)
		: m_floorMesh(BuildFloor(wallHeight))
		, m_ceilingMesh(BuildCeiling(wallHeight))
		, m_wallsMesh(BuildWalls(grid, wallHeight))
	{
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

	static std::vector<Vertex> BuildFloor(float /*wallHeight*/)
	{
		std::vector<Vertex> vertices;

		AddQuad(
			vertices,
			{ 0, 0, 0 },
			{ N, 0, 0 },
			{ N, 0, N },
			{ 0, 0, N },
			{ 0, 1, 0 });

		return vertices;
	}

	static std::vector<Vertex> BuildCeiling(float wallHeight)
	{
		std::vector<Vertex> vertices;

		AddQuad(
			vertices,
			{ 0, wallHeight, N },
			{ N, wallHeight, N },
			{ N, wallHeight, 0 },
			{ 0, wallHeight, 0 },
			{ 0, -1, 0 });

		return vertices;
	}

	static std::vector<Vertex> BuildWalls(const MazeGrid<N>& grid, float wallHeight)
	{
		std::vector<Vertex> vertices;

		for (int row = 0; row < N; ++row)
		{
			for (int col = 0; col < N; ++col)
			{
				if (grid.IsOpen(row, col))
				{
					continue;
				}

				TryAddSouthWall(vertices, grid, row, col, wallHeight);
				TryAddNorthWall(vertices, grid, row, col, wallHeight);
				TryAddEastWall(vertices, grid, row, col, wallHeight);
				TryAddWestWall(vertices, grid, row, col, wallHeight);
			}
		}

		return vertices;
	}

	static void TryAddSouthWall(std::vector<Vertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row + 1, col))
		{
			AddQuad(vertices,
				{ col, 0, row + 1 },
				{ col + 1, 0, row + 1 },
				{ col + 1, wallHeight, row + 1 },
				{ col, wallHeight, row + 1 },
				{ 0, 0, 1 });
		}
	}

	static void TryAddNorthWall(std::vector<Vertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row - 1, col))
		{
			AddQuad(vertices,
				{ col + 1, 0, row },
				{ col, 0, row },
				{ col, wallHeight, row },
				{ col + 1, wallHeight, row },
				{ 0, 0, -1 });
		}
	}

	static void TryAddEastWall(std::vector<Vertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row, col + 1))
		{
			AddQuad(vertices,
				{ col + 1, 0, row + 1 },
				{ col + 1, 0, row },
				{ col + 1, wallHeight, row },
				{ col + 1, wallHeight, row + 1 },
				{ 1, 0, 0 });
		}
	}

	static void TryAddWestWall(std::vector<Vertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row, col - 1))
		{
			AddQuad(vertices,
				{ col, 0, row },
				{ col, 0, row + 1 },
				{ col, wallHeight, row + 1 },
				{ col, wallHeight, row },
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

	Mesh m_floorMesh;
	Mesh m_ceilingMesh;
	Mesh m_wallsMesh;
};