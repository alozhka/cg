#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <stdexcept>
#include <string>

class Texture
{
public:
	explicit Texture(const std::string& filePath)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			throw std::runtime_error("Failed to load texture: " + filePath);
		}

		GLenum format = GL_RGB;
		if (channels == 4)
		{
			format = GL_RGBA;
		}
		else if (channels == 1)
		{
			format = GL_RED;
		}

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}

	~Texture()
	{
		if (m_texture)
		{
			glDeleteTextures(1, &m_texture);
		}
	}

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		: m_texture(other.m_texture)
	{
		other.m_texture = 0;
	}

	void Bind(GLuint unit = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

private:
	GLuint m_texture = 0;
};