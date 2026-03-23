#pragma once
#include "Rectangle3d.hpp"
#include <graphics/Camera.hpp>
#include <graphics/Color.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class PolyhedronApp final : public GraphicsApplication
{
public:
	explicit PolyhedronApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_camera(
			  { 0, 0, 2.5 },
			  { 0, 0, 0 },
			  { 0, 1, 0 })
		, m_rect(1.4, 0.9)

	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
	}

protected:
	void OnDraw(const Mat4& projection) override
	{
		glClearColor(0.08, 0.08, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Mat4 mvp = projection * m_camera.GetViewMatrix();

		m_shader.Use();
		m_shader.SetUniformMat4("uMVP", mvp.ToFloatArray());
		m_shader.SetUniformVec4("uColor", m_color.ToFloatArray());

		m_rect.Draw();
	}

private:
	Camera m_camera;
	ShaderProgram m_shader;
	Color m_color{ 0.2, 0.8, 0.45, 1 };

	Rectangle3d m_rect;
};