#pragma once

#include "Chess.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/TimeProvider.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ChessApp final : public GraphicsApplication
{
public:
	ChessApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_chess(m_textureCache)
		, m_cameraController(m_camera)
	{
		m_shader.LoadFromFile("assets/phong.vert", "assets/phong.frag");
		glEnable(GL_DEPTH_TEST);

		m_camera.SetTarget({ 0, -0.2, 0 });
		m_camera.SetPitchLimits(glm::radians(-80.0), glm::radians(-4.0));
		m_camera.SetRadius(2.5);
		m_camera.AddPitch(glm::radians(-35.0));

		m_light.SetDirection({ -0.4, -1, -0 });
		m_light.SetAmbientIntensity(0.5, 0.5, 0.5);
		m_light.SetDiffuseIntensity(1, 1, 1);
		m_light.SetSpecularIntensity(0.4, 0.4, 0.4);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.1, 0.11, 0.13, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_chess.Update(m_time.GetDeltaTime());

		const glm::mat4 viewProjection = perspective * m_camera.GetViewMatrix();

		m_shader.Use();
		m_shader.SetUniformMat4("uViewProjection", viewProjection);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());
		m_shader.SetUniformInt("uPointLightCount", 0);
		m_light.Apply(m_shader);

		m_chess.Draw(m_shader);
	}

	void OnMouseButton(int button, int action, glm::vec2 p) override
	{
		m_cameraController.OnMouseButton(button, action, p);
	}

	void OnMouseMove(glm::vec2 p) override
	{
		m_cameraController.OnMouseMove(p);
	}

	void OnScroll(double xOffset, double yOffset) override
	{
		m_cameraController.OnScroll(yOffset);
	}

private:
	TextureCache m_textureCache;
	ShaderProgram m_shader;
	OrbitalCamera m_camera;
	DirectLight m_light;
	Chess m_chess;
	CameraController m_cameraController;
	TimeProvider m_time;
};
