#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>

#include <graphics/Texture.hpp>

struct TextureEntry
{
	std::string name;
	std::string path;
};

class TextureLoader
{
public:
	TextureLoader(std::initializer_list<TextureEntry> entries)
	{
		for (const auto& entry : entries)
		{
			m_textures.emplace(entry.name, Texture(entry.path));
		}
	}

	Texture& Get(const std::string& name) { return m_textures.at(name); }

private:
	std::unordered_map<std::string, Texture> m_textures;
};