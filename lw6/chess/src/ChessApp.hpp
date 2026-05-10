#pragma once

#include <graphics/Model.hpp>
#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class ChessApp final : public GraphicsApplication
{
public:
	ChessApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_king("assets/chess/king.obj", m_textureCache)
		, m_cameraController(m_camera)
	{
		m_shader.LoadFromFile("assets/phong.vert", "assets/phong.frag");
		glEnable(GL_DEPTH_TEST);

		m_camera.SetTarget({ 0, 0.2, 0 });
		m_camera.SetRadius(1.2);
		m_camera.AddPitch(glm::radians(15.0));

		m_light.SetDirection({ -0.4, -1, -0 });
		m_light.SetAmbientIntensity(0.3, 0.3, 0.3);
		m_light.SetDiffuseIntensity(0.8, 0.8, 0.8);
		m_light.SetSpecularIntensity(0.4, 0.4, 0.4);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.1, 0.11, 0.13, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const glm::mat4 view = m_camera.GetViewMatrix();
		const glm::mat4 viewProjection = perspective * view;
		const glm::mat4 model(1);
		const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

		m_shader.Use();
		m_shader.SetUniformMat4("uViewProjection", viewProjection);
		m_shader.SetUniformMat4("uModel", model);
		m_shader.SetUniformMat3("uNormalMatrix", normalMatrix);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());
		m_shader.SetUniformInt("uPointLightCount", 0);
		m_light.Apply(m_shader);

		m_king.Draw(m_shader);
	}

	void OnMouseButton(int button, int action, glm::vec2 p) override
	{
		m_cameraController.OnMouseButton(button, action, p);
	}

	void OnMouseMove(glm::vec2 p) override
	{
		m_cameraController.OnMouseMove(p);
	}

private:
	TextureCache m_textureCache;
	ShaderProgram m_shader;
	OrbitalCamera m_camera;
	DirectLight m_light;
	Model m_king;
	CameraController m_cameraController;
};