#pragma once

#include <array>
#include <graphics/Texture.hpp>
#include <graphics/TexturedMesh.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

class TextureMazeMesh
{
public:
	TextureMazeMesh()
	{
		LoadTextures();
		BuildGeometry();
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.SetUniformMat4("uModel", glm::mat4(1.0f));
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformInt("uTexture", 0);

		m_floorTexture->Bind(0);
		m_floorMesh->Draw(GL_TRIANGLES);

		m_ceilingTexture->Bind(0);
		m_ceilingMesh->Draw(GL_TRIANGLES);

		for (int i = 0; i < NUM_WALL_TEXTURES; ++i)
		{
			if (m_wallMeshes[i])
			{
				m_wallTextures[i]->Bind(0);
				m_wallMeshes[i]->Draw(GL_TRIANGLES);
			}
		}
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
	static constexpr int NUM_WALL_TEXTURES = 6;

	void LoadTextures()
	{
		const std::array<std::string, NUM_WALL_TEXTURES> wallTextureFiles = {
			"assets/textures/brick.jpg",
			"assets/textures/stone.jpg",
			"assets/textures/wood.jpg",
			"assets/textures/concrete.jpg",
			"assets/textures/tile.jpg",
			"assets/textures/golden_freddie.jpg",
		};

		for (int i = 0; i < NUM_WALL_TEXTURES; ++i)
		{
			m_wallTextures[i] = std::make_unique<Texture>(wallTextureFiles[i]);
		}

		m_floorTexture = std::make_unique<Texture>("assets/textures/floor.jpg");
		m_ceilingTexture = std::make_unique<Texture>("assets/textures/ceiling.jpg");
	}

	void BuildGeometry()
	{
		std::array<std::vector<TexturedVertex>, NUM_WALL_TEXTURES> wallVertices;
		std::vector<TexturedVertex> floorVertices;
		std::vector<TexturedVertex> ceilingVertices;

		BuildFloor(floorVertices);
		BuildCeiling(ceilingVertices);
		BuildWalls(wallVertices);

		m_floorMesh = std::make_unique<TexturedMesh>(floorVertices);
		m_ceilingMesh = std::make_unique<TexturedMesh>(ceilingVertices);

		for (int i = 0; i < NUM_WALL_TEXTURES; ++i)
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

	static void BuildFloor(std::vector<TexturedVertex>& vertices)
	{
		AddQuad(
			vertices,
			{ 0, 0, 0 },
			{ MAZE_CELLS, 0, 0 },
			{ MAZE_CELLS, 0, MAZE_CELLS },
			{ 0, 0, MAZE_CELLS },
			{ 0, 1, 0 },
			{ 0, 0 },
			{ MAZE_CELLS, 0 },
			{ MAZE_CELLS, MAZE_CELLS },
			{ 0, MAZE_CELLS });
	}

	static void BuildCeiling(std::vector<TexturedVertex>& vertices)
	{
		AddQuad(
			vertices,
			{ 0, WALL_HEIGHT, MAZE_CELLS },
			{ MAZE_CELLS, WALL_HEIGHT, MAZE_CELLS },
			{ MAZE_CELLS, WALL_HEIGHT, 0 },
			{ 0, WALL_HEIGHT, 0 },
			{ 0, -1, 0 },
			{ 0, MAZE_CELLS },
			{ MAZE_CELLS, MAZE_CELLS },
			{ MAZE_CELLS, 0 },
			{ 0, 0 });
	}

	static int GetWallTextureIndex(int row, int col)
	{
		return (row * 7 + col * 13) % NUM_WALL_TEXTURES;
	}

	static void BuildWalls(std::array<std::vector<TexturedVertex>, NUM_WALL_TEXTURES>& wallVertices)
	{
		for (int row = 0; row < MAZE_CELLS; ++row)
		{
			for (int col = 0; col < MAZE_CELLS; ++col)
			{
				if (MAZE_GRID[row][col] == 0)
				{
					continue;
				}

				TryAddSouthWall(wallVertices, row, col);
				TryAddNorthWall(wallVertices, row, col);
				TryAddEastWall(wallVertices, row, col);
				TryAddWestWall(wallVertices, row, col);
			}
		}
	}

	static void TryAddSouthWall(std::array<std::vector<TexturedVertex>, NUM_WALL_TEXTURES>& wallVertices, int row, int col)
	{
		if (IsOpenBlock(row + 1, col))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col, 0, row + 1 },
				{ col + 1, 0, row + 1 },
				{ col + 1, WALL_HEIGHT, row + 1 },
				{ col, WALL_HEIGHT, row + 1 },
				{ 0, 0, 1 },
				{ 0, 0 },
				{ 1, 0 },
				{ 1, 1 },
				{ 0, 1 });
		}
	}

	static void TryAddNorthWall(std::array<std::vector<TexturedVertex>, NUM_WALL_TEXTURES>& wallVertices, int row, int col)
	{
		if (IsOpenBlock(row - 1, col))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col + 1, 0, row },
				{ col, 0, row },
				{ col, WALL_HEIGHT, row },
				{ col + 1, WALL_HEIGHT, row },
				{ 0, 0, -1 },
				{ 0, 0 },
				{ 1, 0 },
				{ 1, 1 },
				{ 0, 1 });
		}
	}

	static void TryAddEastWall(std::array<std::vector<TexturedVertex>, NUM_WALL_TEXTURES>& wallVertices, int row, int col)
	{
		if (IsOpenBlock(row, col + 1))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col + 1, 0, row + 1 },
				{ col + 1, 0, row },
				{ col + 1, WALL_HEIGHT, row },
				{ col + 1, WALL_HEIGHT, row + 1 },
				{ 1, 0, 0 },
				{ 0, 0 },
				{ 1, 0 },
				{ 1, 1 },
				{ 0, 1 });
		}
	}

	static void TryAddWestWall(std::array<std::vector<TexturedVertex>, NUM_WALL_TEXTURES>& wallVertices, int row, int col)
	{
		if (IsOpenBlock(row, col - 1))
		{
			int idx = GetWallTextureIndex(row, col);
			AddQuad(wallVertices[idx],
				{ col, 0, row },
				{ col, 0, row + 1 },
				{ col, WALL_HEIGHT, row + 1 },
				{ col, WALL_HEIGHT, row },
				{ -1, 0, 0 },
				{ 0, 0 },
				{ 1, 0 },
				{ 1, 1 },
				{ 0, 1 });
		}
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
	std::array<std::unique_ptr<TexturedMesh>, NUM_WALL_TEXTURES> m_wallMeshes;

	std::array<std::unique_ptr<Texture>, NUM_WALL_TEXTURES> m_wallTextures;
	std::unique_ptr<Texture> m_floorTexture;
	std::unique_ptr<Texture> m_ceilingTexture;

	std::unique_ptr<TexturedMesh> m_floorMesh;
	std::unique_ptr<TexturedMesh> m_ceilingMesh;
};