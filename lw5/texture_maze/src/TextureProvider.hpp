#pragma once

#include <array>
#include <memory>
#include <string>

#include <graphics/Texture.hpp>

class TextureProvider
{
public:
	static constexpr int NUM_WALL_TEXTURES = 6;

	TextureProvider()
	{
		const std::array<std::string, NUM_WALL_TEXTURES> wallFiles = {
			"assets/textures/brick.jpg",
			"assets/textures/stone.jpg",
			"assets/textures/wood.jpg",
			"assets/textures/concrete.jpg",
			"assets/textures/tile.jpg",
			"assets/textures/golden_freddie.jpg",
		};

		for (int i = 0; i < NUM_WALL_TEXTURES; ++i)
		{
			m_wallTextures[i] = std::make_unique<Texture>(wallFiles[i]);
		}

		m_floorTexture = std::make_unique<Texture>("assets/textures/floor.jpg");
		m_ceilingTexture = std::make_unique<Texture>("assets/textures/ceiling.jpg");
	}

	Texture& GetWallTexture(int index) const { return *m_wallTextures[index]; }
	Texture& GetFloorTexture() const { return *m_floorTexture; }
	Texture& GetCeilingTexture() const { return *m_ceilingTexture; }

private:
	std::array<std::unique_ptr<Texture>, NUM_WALL_TEXTURES> m_wallTextures;
	std::unique_ptr<Texture> m_floorTexture;
	std::unique_ptr<Texture> m_ceilingTexture;
};