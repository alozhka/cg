#pragma once

#include "MazeGrid.hpp"

#include <graphics/TexturedMesh.hpp>
#include <graphics/textures/TextureLoader.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <string>
#include <vector>

template <int N>
class TextureMazeMesh
{
public:
	TextureMazeMesh(const MazeGrid<N>& grid, float wallHeight, TextureLoader& textures)
		: m_textures(textures)
		, m_wallMesh(BuildWalls(grid, wallHeight))
		, m_floorMesh(BuildFloor())
		, m_ceilingMesh(BuildCeiling(wallHeight))
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformMat4("uModel", glm::mat4(1));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformInt("uTexture", 0);

		m_textures.Get("floor").Bind(0);
		m_floorMesh.Draw(GL_TRIANGLES);

		m_textures.Get("ceiling").Bind(0);
		m_ceilingMesh.Draw(GL_TRIANGLES);

		for (const auto& segment : m_walls)
		{
			m_textures.Get(segment.textureKey).Bind(0);
			m_wallMesh.Draw(GL_TRIANGLES, segment.first, segment.count);
		}
	}

private:
	static std::vector<TexturedVertex> BuildFloor()
	{
		std::vector<TexturedVertex> vertices;

		AddQuad(
			vertices,
			{ 0, 0, 0 },
			{ N, 0, 0 },
			{ N, 0, N },
			{ 0, 0, N },
			{ 0, 1, 0 });

		return vertices;
	}

	static std::vector<TexturedVertex> BuildCeiling(float wallHeight)
	{
		std::vector<TexturedVertex> vertices;

		AddQuad(
			vertices,
			{ 0, wallHeight, N },
			{ N, wallHeight, N },
			{ N, wallHeight, 0 },
			{ 0, wallHeight, 0 },
			{ 0, -1, 0 });

		return vertices;
	}

	std::vector<TexturedVertex> BuildWalls(const MazeGrid<N>& grid, float wallHeight)
	{
		std::vector<TexturedVertex> vertices;

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

	void TryAddSouthWall(std::vector<TexturedVertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row + 1, col))
		{
			m_walls.push_back({ GetWallTextureKey(row, col), static_cast<GLint>(vertices.size()), 6 });

			AddQuad(vertices,
				{ col, 0, row + 1 },
				{ col + 1, 0, row + 1 },
				{ col + 1, wallHeight, row + 1 },
				{ col, wallHeight, row + 1 },
				{ 0, 0, 1 });
		}
	}

	void TryAddNorthWall(std::vector<TexturedVertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row - 1, col))
		{
			m_walls.push_back({ GetWallTextureKey(row, col), static_cast<GLint>(vertices.size()), 6 });

			AddQuad(vertices,
				{ col + 1, 0, row },
				{ col, 0, row },
				{ col, wallHeight, row },
				{ col + 1, wallHeight, row },
				{ 0, 0, -1 });
		}
	}

	void TryAddEastWall(std::vector<TexturedVertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row, col + 1))
		{
			m_walls.push_back({ GetWallTextureKey(row, col), static_cast<GLint>(vertices.size()), 6 });

			AddQuad(vertices,
				{ col + 1, 0, row + 1 },
				{ col + 1, 0, row },
				{ col + 1, wallHeight, row },
				{ col + 1, wallHeight, row + 1 },
				{ 1, 0, 0 });
		}
	}

	void TryAddWestWall(std::vector<TexturedVertex>& vertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row, col - 1))
		{
			m_walls.push_back({ GetWallTextureKey(row, col), static_cast<GLint>(vertices.size()), 6 });

			AddQuad(vertices,
				{ col, 0, row },
				{ col, 0, row + 1 },
				{ col, wallHeight, row + 1 },
				{ col, wallHeight, row },
				{ -1, 0, 0 });
		}
	}

	std::string GetWallTextureKey(int row, int col) const
	{
		int idx = (row * 7 + col * 13) % m_wallTextureKeys.size();
		return m_wallTextureKeys[idx];
	}

	static void AddQuad(
		std::vector<TexturedVertex>& vertices,
		glm::vec3 a,
		glm::vec3 b,
		glm::vec3 c,
		glm::vec3 d,
		glm::vec3 normal)
	{
		float width = glm::length(b - a);
		float height = glm::length(d - a);

		glm::vec2 uv0 = { 0, 0 };
		glm::vec2 uv1 = { width, 0 };
		glm::vec2 uv2 = { width, height };
		glm::vec2 uv3 = { 0, height };

		vertices.push_back({ a, normal, uv0 });
		vertices.push_back({ b, normal, uv1 });
		vertices.push_back({ c, normal, uv2 });
		vertices.push_back({ a, normal, uv0 });
		vertices.push_back({ c, normal, uv2 });
		vertices.push_back({ d, normal, uv3 });
	}

	struct WallSegment
	{
		std::string textureKey;
		GLint first = 0;
		GLsizei count = 0;
	};

	TextureLoader& m_textures;
	std::vector<std::string> m_wallTextureKeys = { "wall_0", "wall_1", "wall_2", "wall_3", "wall_4", "wall_5" };
	std::vector<WallSegment> m_walls;
	TexturedMesh m_wallMesh;
	TexturedMesh m_floorMesh;
	TexturedMesh m_ceilingMesh;
};