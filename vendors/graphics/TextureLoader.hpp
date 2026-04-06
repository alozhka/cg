#pragma once
#include "Texture.hpp"

#include <unordered_map>

class TextureLoader
{
private:
	std::unordered_map<std::string, Texture> m_textures;
};