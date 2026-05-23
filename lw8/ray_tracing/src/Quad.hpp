#pragma once

#include "FrameBuffer.hpp"

#include <graphics/TexturedMesh.hpp>

#include <glad/glad.h>

#include <vector>

class Quad
{
public:
	Quad(unsigned width, unsigned height)
		: m_width(width)
		, m_height(height)
		, m_mesh(BuildVertices())
	{
		InitTexture();
	}

	~Quad()
	{
		if (m_texture)
		{
			glDeleteTextures(1, &m_texture);
		}
	}

	Quad(const Quad&) = delete;
	Quad& operator=(const Quad&) = delete;

	void Upload(const FrameBuffer& frame)
	{
		// FrameBuffer хранит пиксели в формате 0xAARRGGBB. В памяти little-endian
		// это байты B, G, R, A — соответствует GL_BGRA + GL_UNSIGNED_BYTE.
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			static_cast<GLsizei>(m_width),
			static_cast<GLsizei>(m_height),
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			frame.GetPixels(0));
	}

	void Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		m_mesh.Draw(GL_TRIANGLES);
	}

private:
	static std::vector<TexturedVertex> BuildVertices()
	{
		constexpr glm::vec3 n{ 0, 0, 1 };
		return {
			{ { -1, -1, 0 }, n, { 0, 1 } },
			{ { 1, -1, 0 }, n, { 1, 1 } },
			{ { 1, 1, 0 }, n, { 1, 0 } },
			{ { -1, -1, 0 }, n, { 0, 1 } },
			{ { 1, 1, 0 }, n, { 1, 0 } },
			{ { -1, 1, 0 }, n, { 0, 0 } },
		};
	}

	void InitTexture()
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			static_cast<GLsizei>(m_width),
			static_cast<GLsizei>(m_height),
			0,
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned m_width;
	unsigned m_height;
	TexturedMesh m_mesh;
	GLuint m_texture = 0;
};