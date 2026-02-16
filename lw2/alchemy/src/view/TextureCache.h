#pragma once

#include "../model/Element.h"

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <unordered_map>

class TextureCache
{
public:
	explicit TextureCache(const std::string& assetsDir)
		: m_assetsDir(assetsDir)
	{
	}

	const sf::Texture* Get(ElementType type)
	{
		auto it = m_cache.find(type);
		if (it != m_cache.end())
		{
			return it->second.get();
		}

		std::string path = m_assetsDir + "/" + ElementTypeToImageFilename(type);

		if (!std::filesystem::exists(path))
		{
			m_cache[type] = nullptr;
			return nullptr;
		}

		auto texture = std::make_unique<sf::Texture>(path);
		texture->setSmooth(true);
		const sf::Texture* ptr = texture.get();
		m_cache[type] = std::move(texture);
		return ptr;
	}

private:
	std::string m_assetsDir;
	std::unordered_map<ElementType, std::unique_ptr<sf::Texture>> m_cache;
};
