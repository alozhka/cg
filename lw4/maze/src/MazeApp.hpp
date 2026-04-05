#pragma once

#include "CollisionDetector.hpp"
#include "Maze.hpp"

#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/FirstPersonCameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class MazeApp final : public GraphicsApplication
{
public:
	MazeApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_cameraController(m_camera)
		, m_keyboard(CreateKeyboardReader())
		, m_light({ 0, 1, 0 })
	{
		m_zfar = 100.f;

		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		glEnable(GL_DEPTH_TEST);
		CaptureMouseInput();

		m_camera.SetPosition({ 1.5, 0.5, 1.5 });

		m_light.SetAmbientIntensity(0.3, 0.3, 0.3);
		m_light.SetDiffuseIntensity(0.6, 0.6, 0.6);
		m_light.SetSpecularIntensity(0.2, 0.2, 0.2);

		m_lastTime = static_cast<float>(glfwGetTime());
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		UpdateMovement();

		glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 viewProjection = perspective * view;

		m_shader.Use();
		m_light.SetDirection(-m_camera.GetForwardXZ());
		m_light.Apply(m_shader);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());

		m_maze.Draw(m_shader, viewProjection);
	}

	void OnRawMouseMove(double x, double y) override
	{
		m_cameraController.OnRawMouseMove(x, y);
	}

private:
	void UpdateMovement()
	{
		float now = static_cast<float>(glfwGetTime());
		float dt = now - m_lastTime;
		m_lastTime = now;

		glm::vec3 forward = m_camera.GetForwardXZ();
		glm::vec3 right = m_camera.GetRightXZ();

		glm::vec3 move(0);

		if (m_keyboard.IsButtonPressed(GLFW_KEY_W))
			move += forward;
		if (m_keyboard.IsButtonPressed(GLFW_KEY_S))
			move -= forward;
		if (m_keyboard.IsButtonPressed(GLFW_KEY_D))
			move += right;
		if (m_keyboard.IsButtonPressed(GLFW_KEY_A))
			move -= right;

		move = glm::normalize(move) * MOVE_SPEED * dt;

		glm::vec3 pos = m_camera.GetPosition();
		m_camera.SetPosition(CollisionDetector::ResolveMovement(pos, move));
	}

	static constexpr float MOVE_SPEED = 3.0f;

	FirstPersonCamera m_camera;
	FirstPersonCameraController m_cameraController;
	KeyboardReader m_keyboard;
	ShaderProgram m_shader;
	DirectLight m_light;
	Maze m_maze;
	float m_lastTime = 0.f;
};