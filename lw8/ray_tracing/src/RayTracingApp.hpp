#pragma once

#include "objects/Quad.hpp"
#include "render/FrameBuffer.hpp"
#include "render/Renderer.hpp"
#include "shading/Shading.hpp"

#include "core/Ray.hpp"
#include "core/Scene.hpp"
#include "scene/PyramidScene.hpp"
#include "scene/TeapotScene.hpp"

#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/FirstPersonCameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/KeyboardCameraController.hpp>
#include <graphics/windows/KeyboardReader.hpp>
#include <graphics/windows/TimeProvider.hpp>

#include <GLFW/glfw3.h>
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

		StartRender();
	}

	~RayTracingApp() override
	{
		m_renderer.Stop();
	}

protected:
	void OnDraw(const glm::mat4&) override
	{
		m_keyboardController.Update(m_time.GetDeltaTime());
		// Сначала презентуем готовый кадр (если есть), потом думаем о рестарте.
		// Иначе следующий StartRender перебьёт флаг и кадр уйдёт в небытие.
		PresentFrameIfRendered();
		RestartRenderIfReady();

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
		TeapotScene::Build(m_scene);
	}

	void InitCamera()
	{
		m_camera.SetPosition(glm::vec3{ 0, 1, 5 });
		m_camera.AddYaw(glm::radians(-90.f));
	}

	void PresentFrameIfRendered()
	{
		if (m_renderer.IsRendering())
		{
			return;
		}
		m_quad.Upload(m_frameBuffer);
	}

	void RestartRenderIfReady()
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

		StartRender();
	}

	void StartRender()
	{
		m_prevPosition = m_camera.GetPosition();
		m_prevForward = m_camera.GetForward();

		const FirstPersonCamera cameraSnap = m_camera;
		const float fov = m_fieldOfView;
		const float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
		const Scene& scene = m_scene;

		m_renderer.Render(m_frameBuffer,
			[&scene, cameraSnap, fov, aspect](int x, int y, int w, int h) -> std::uint32_t {
				// Центр пикселя в нормированных координатах с v вверх.
				const float u = (2.f * (static_cast<float>(x) + 0.5f) / static_cast<float>(w)) - 1.f;
				const float v = 1.f - (2.f * (static_cast<float>(y) + 0.5f) / static_cast<float>(h));
				const Ray ray = Ray::Primary(cameraSnap, u, v, fov, aspect);
				return Shading::PackColor(Shading::TraceRay(scene, ray));
			});
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