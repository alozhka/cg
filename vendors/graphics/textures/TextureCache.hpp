#pragma once

#include <graphics/Texture.hpp>

#include <memory>
#include <string>
#include <unordered_map>

class TextureCache
{
public:
	TextureCache() = default;

	TextureCache(const TextureCache&) = delete;
	TextureCache& operator=(const TextureCache&) = delete;

	const Texture& Load(const std::string& path)
	{
		auto it = m_textures.find(path);
		if (it != m_textures.end())
		{
			return *it->second;
		}
		auto [inserted, _] = m_textures.emplace(path, std::make_unique<Texture>(path));
		return *inserted->second;
	}

private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};