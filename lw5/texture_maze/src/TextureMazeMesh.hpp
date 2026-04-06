#pragma once

#include "MazeGrid.hpp"
#include "TextureProvider.hpp"

#include <array>
#include <graphics/TexturedMesh.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

template<int N>
class TextureMazeMesh
{
public:
	TextureMazeMesh(const MazeGrid<N>& grid, float wallHeight)
	{
		BuildGeometry(grid, wallHeight);
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformInt("uTexture", 0);

		m_textures.GetFloorTexture().Bind(0);
		m_floorMesh->Draw(GL_TRIANGLES);

		m_textures.GetCeilingTexture().Bind(0);
		m_ceilingMesh->Draw(GL_TRIANGLES);

		for (int i = 0; i < TextureProvider::NUM_WALL_TEXTURES; ++i)
		{
			if (m_wallMeshes[i])
			{
				m_textures.GetWallTexture(i).Bind(0);
				m_wallMeshes[i]->Draw(GL_TRIANGLES);
			}
		}
	}

private:
	void BuildGeometry(const MazeGrid<N>& grid, float wallHeight)
	{
		std::array<std::vector<TexturedVertex>, TextureProvider::NUM_WALL_TEXTURES> wallVertices;
		std::vector<TexturedVertex> floorVertices;
		std::vector<TexturedVertex> ceilingVertices;

		BuildFloor(floorVertices, wallHeight);
		BuildCeiling(ceilingVertices, wallHeight);
		BuildWalls(wallVertices, grid, wallHeight);

		m_floorMesh = std::make_unique<TexturedMesh>(floorVertices);
		m_ceilingMesh = std::make_unique<TexturedMesh>(ceilingVertices);

		for (int i = 0; i < TextureProvider::NUM_WALL_TEXTURES; ++i)
		{
			if (!wallVertices[i].empty())
			{
				m_wallMeshes[i] = std::make_unique<TexturedMesh>(wallVertices[i]);
			}
		}
	}

	static void AddQuad(std::vector<TexturedVertex>& vertices,
		glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
		glm::vec3 normal,
		glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
	{
		vertices.push_back({ a, normal, uv0 });
		vertices.push_back({ b, normal, uv1 });
		vertices.push_back({ c, normal, uv2 });
		vertices.push_back({ a, normal, uv0 });
		vertices.push_back({ c, normal, uv2 });
		vertices.push_back({ d, normal, uv3 });
	}

	static void BuildFloor(std::vector<TexturedVertex>& vertices, float /*wallHeight*/)
	{
		AddQuad(
			vertices,
			{ 0, 0, 0 },
			{ N, 0, 0 },
			{ N, 0, N },
			{ 0, 0, N },
			{ 0, 1, 0 },
			{ 0, 0 },
			{ N, 0 },
			{ N, N },
			{ 0, N });
	}

	static void BuildCeiling(std::vector<TexturedVertex>& vertices, float wallHeight)
	{
		AddQuad(
			vertices,
			{ 0, wallHeight, N },
			{ N, wallHeight, N },
			{ N, wallHeight, 0 },
			{ 0, wallHeight, 0 },
			{ 0, -1, 0 },
			{ 0, N },
			{ N, N },
			{ N, 0 },
			{ 0, 0 });
	}

	static int GetWallTextureIndex(int row, int col)
	{
		return (row * 7 + col * 13) % TextureProvider::NUM_WALL_TEXTURES;
	}

	static void BuildWalls(std::array<std::vector<TexturedVertex>, TextureProvider::NUM_WALL_TEXTURES>& wallVertices, const MazeGrid<N>& grid, float wallHeight)
	{
		for (int row = 0; row < N; ++row)
		{
			for (int col = 0; col < N; ++col)
			{
				if (grid.IsOpen(row, col))
				{
					continue;
				}

				TryAddSouthWall(wallVertices, grid, row, col, wallHeight);
				TryAddNorthWall(wallVertices, grid, row, col, wallHeight);
				TryAddEastWall(wallVertices, grid, row, col, wallHeight);
				TryAddWestWall(wallVertices, grid, row, col, wallHeight);
			}
		}
	}

	static void TryAddSouthWall(std::array<std::vector<TexturedVertex>, TextureProvider::NUM_WALL_TEXTURES>& wallVertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row + 1, col))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col, 0, row + 1 },
				{ col + 1, 0, row + 1 },
				{ col + 1, wallHeight, row + 1 },
				{ col, wallHeight, row + 1 },
				{ 0, 0, 1 },
				{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 });
		}
	}

	static void TryAddNorthWall(std::array<std::vector<TexturedVertex>, TextureProvider::NUM_WALL_TEXTURES>& wallVertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row - 1, col))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col + 1, 0, row },
				{ col, 0, row },
				{ col, wallHeight, row },
				{ col + 1, wallHeight, row },
				{ 0, 0, -1 },
				{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 });
		}
	}

	static void TryAddEastWall(std::array<std::vector<TexturedVertex>, TextureProvider::NUM_WALL_TEXTURES>& wallVertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row, col + 1))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col + 1, 0, row + 1 },
				{ col + 1, 0, row },
				{ col + 1, wallHeight, row },
				{ col + 1, wallHeight, row + 1 },
				{ 1, 0, 0 },
				{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 });
		}
	}

	static void TryAddWestWall(std::array<std::vector<TexturedVertex>, TextureProvider::NUM_WALL_TEXTURES>& wallVertices, const MazeGrid<N>& grid, int row, int col, float wallHeight)
	{
		if (grid.IsOpen(row, col - 1))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col, 0, row },
				{ col, 0, row + 1 },
				{ col, wallHeight, row + 1 },
				{ col, wallHeight, row },
				{ -1, 0, 0 },
				{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 });
		}
	}

	TextureProvider m_textures;
	std::array<std::unique_ptr<TexturedMesh>, TextureProvider::NUM_WALL_TEXTURES> m_wallMeshes;
	std::unique_ptr<TexturedMesh> m_floorMesh;
	std::unique_ptr<TexturedMesh> m_ceilingMesh;
};