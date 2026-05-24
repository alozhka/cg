#pragma once

#include "core/Scene.hpp"
#include "objects/Quad.hpp"
#include "render/FrameBuffer.hpp"
#include "render/Renderer.hpp"
#include "scene/PyramidScene.hpp"
#include "scene/TeapotScene.hpp"

#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/FirstPersonCameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/KeyboardCameraController.hpp>
#include <graphics/windows/KeyboardReader.hpp>
#include <graphics/windows/TimeProvider.hpp>

#include <glad/glad.h>

#include <string>

enum class RenderMode
{
	Raster = 0,
	RayTracing = 1,
};

class RayTracingApp : public GraphicsApplication
{
public:
	RayTracingApp(unsigned width, unsigned height, const std::string& title)
		: GraphicsApplication(static_cast<int>(width), static_cast<int>(height), title)
		, m_width(width)
		, m_height(height)
		, m_cameraController(m_camera)
		, m_keys(CreateKeyboardReader())
		, m_keyboardController(m_camera, m_keys)
		, m_frameBuffer(width, height)
		, m_quad(width, height)
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		m_quad.Upload(m_frameBuffer);
		BuildScene();
		InitCamera();
		CaptureMouseInput();
	}

	~RayTracingApp() override
	{
		m_renderer.Stop();
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		m_keyboardController.Update(m_time.GetDeltaTime());

		PresentFrameIfRendered();

		const glm::mat4 viewProjection = perspective * m_camera.GetViewMatrix();
		RestartRenderIfReady(viewProjection);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader.Use();
		m_shader.SetUniformInt("uTex", 0);
		m_quad.Draw();
	}

	void OnRawMouseMove(double x, double y) override
	{
		m_cameraController.OnRawMouseMove(x, y);
	}

private:
	void BuildScene()
	{
		PyramidScene::Build(m_scene);
		// лагает с ним капец
		// TeapotScene::Build(m_scene);
	}

	void InitCamera()
	{
		m_camera.SetPosition(glm::vec3{ 0, 1, 5 });
		m_camera.AddYaw(glm::radians<float>(-90));
	}

	void PresentFrameIfRendered()
	{
		if (m_renderer.IsRendering())
		{
			return;
		}
		m_quad.Upload(m_frameBuffer);
	}

	void RestartRenderIfReady(const glm::mat4& viewProjection)
	{
		if (m_renderer.IsRendering())
		{
			return;
		}

		const glm::vec3 pos = m_camera.GetPosition();
		const glm::vec3 fwd = m_camera.GetForward();
		if (pos == m_prevPosition && fwd == m_prevForward)
		{
			return;
		}

		StartRender(viewProjection);
	}

	void StartRender(const glm::mat4& viewProjection)
	{
		m_prevPosition = m_camera.GetPosition();
		m_prevForward = m_camera.GetForward();

		m_renderer.Render(m_frameBuffer, m_camera, m_scene, viewProjection);
	}

	unsigned m_width;
	unsigned m_height;

	Scene m_scene;
	FirstPersonCamera m_camera;
	FirstPersonCameraController m_cameraController;
	KeyboardReader m_keys;
	KeyboardCameraController m_keyboardController;
	TimeProvider m_time;

	glm::vec3 m_prevPosition{ 0 };
	glm::vec3 m_prevForward{ 0 };

	FrameBuffer m_frameBuffer;
	Renderer m_renderer;

	Quad m_quad;
	ShaderProgram m_shader;
};