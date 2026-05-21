#pragma once

#include "FrameBuffer.hpp"
#include "Renderer.hpp"

#include <graphics/TexturedMesh.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

#include <glad/glad.h>

#include <string>
#include <vector>

class RayTracingApp : public GraphicsApplication
{
public:
	RayTracingApp(unsigned width, unsigned height, const std::string& title)
		: GraphicsApplication(static_cast<int>(width), static_cast<int>(height), title)
		, m_width(width)
		, m_height(height)
		, m_frameBuffer(width, height)
		, m_quad(BuildQuadVertices())
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		InitTexture();
		m_renderer.Render(m_frameBuffer);
	}

	~RayTracingApp() override
	{
		m_renderer.Stop();
		if (m_texture)
		{
			glDeleteTextures(1, &m_texture);
		}
	}

protected:
	void OnDraw(const glm::mat4& /*perspective*/) override
	{
		UploadFrameBuffer();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		m_shader.SetUniformInt("uTex", 0);

		m_quad.Draw(GL_TRIANGLES);
	}

private:
	static std::vector<TexturedVertex> BuildQuadVertices()
	{
		// Полноэкранный квад: позиции в NDC, normal не используется,
		// UV.y = 0 наверху — чтобы строка 0 framebuffer'а оказалась наверху экрана.
		const glm::vec3 n{ 0.f, 0.f, 1.f };
		return {
			{ { -1.f, -1.f, 0.f }, n, { 0.f, 1.f } },
			{ {  1.f, -1.f, 0.f }, n, { 1.f, 1.f } },
			{ {  1.f,  1.f, 0.f }, n, { 1.f, 0.f } },
			{ { -1.f, -1.f, 0.f }, n, { 0.f, 1.f } },
			{ {  1.f,  1.f, 0.f }, n, { 1.f, 0.f } },
			{ { -1.f,  1.f, 0.f }, n, { 0.f, 0.f } },
		};
	}

	void InitTexture()
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA8,
			static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height),
			0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void UploadFrameBuffer()
	{
		// Framebuffer хранит пиксели в формате 0xAARRGGBB. В памяти little-endian
		// это байты B, G, R, A — соответствует GL_BGRA + GL_UNSIGNED_BYTE.
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexSubImage2D(
			GL_TEXTURE_2D, 0, 0, 0,
			static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height),
			GL_BGRA, GL_UNSIGNED_BYTE, m_frameBuffer.GetPixels(0));
	}

	unsigned m_width;
	unsigned m_height;

	FrameBuffer m_frameBuffer;
	Renderer m_renderer;

	TexturedMesh m_quad;
	ShaderProgram m_shader;
	GLuint m_texture = 0;
};