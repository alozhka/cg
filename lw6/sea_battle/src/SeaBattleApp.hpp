#pragma once
#include <graphics/DrawableModel.hpp>
#include <graphics/Model.hpp>
#include <graphics/ObjLoader.hpp>
#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class SeaBattleApp final : public GraphicsApplication
{
public:
	SeaBattleApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_boat1(ObjLoader::Load("assets/cruiser/24RPMGBXX36TDDT35PAGXDXJI.obj", m_textureCache))
		, m_boat(m_boat1)
	{

		m_shader.LoadFromFile("assets/phong.vert", "assets/phong.frag");
		CaptureMouseInput();
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.1, 0.11, 0.13, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const glm::mat4 viewProjection = perspective * m_camera.GetViewMatrix();

		m_shader.Use();
		m_shader.SetUniformMat4("uViewProjection", viewProjection);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());
		m_shader.SetUniformInt("uPointLightCount", 0);

		m_boat.Draw(m_shader, viewProjection);
	}

private:
	TextureCache m_textureCache;
	Model m_boat1;
	DrawableModel m_boat;
	ShaderProgram m_shader;
	FirstPersonCamera m_camera;
};