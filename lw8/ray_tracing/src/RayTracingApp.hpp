#pragma once

#include "core/Scene.hpp"
#include "objects/Quad.hpp"
#include "render/FrameBuffer.hpp"
#include "render/Renderer.hpp"
#include "scene/PyramidScene.hpp"
#include "scene/TeapotScene.hpp"

#include <graphics/CompositeDrawable3D.hpp>
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
	RayTracingApp(unsigned width, unsigned height)
		: GraphicsApplication(static_cast<int>(width), static_cast<int>(height), "Ray Tracing")
		, m_width(width)
		, m_height(height)
		, m_cameraController(m_camera)
		, m_keys(CreateKeyboardReader())
		, m_keyboardController(m_camera, m_keys)
		, m_frameBuffer(width, height)
		, m_quad(width, height)
	{
		m_quadShader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		m_rasterShader.LoadFromFile("assets/raster_vertex.glsl", "assets/raster_fragment.glsl");

		glEnable(GL_DEPTH_TEST);
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
		UpdateTitle();
		const glm::mat4 viewProjection = perspective * m_camera.GetViewMatrix();
		HandleInput(viewProjection);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw(viewProjection);
	}

	void OnRawMouseMove(double x, double y) override
	{
		if (m_renderMode == RenderMode::RayTracing)
		{
			return;
		}
		m_cameraController.OnRawMouseMove(x, y);
	}

private:
	void Draw(const glm::mat4& viewProjection)
	{
		if (m_renderMode == RenderMode::Raster || m_renderer.IsRendering())
		{
			m_rasterShader.SetUniformMat4("uViewProjection", viewProjection);
			m_rasterShader.SetUniformVec3("uCameraPos", m_camera.GetPosition());
			m_rasterShader.Use();
			m_rasterScene.Draw(m_rasterShader, glm::mat4{ 1 });
		}
		else
		{
			m_quad.Upload(m_frameBuffer);

			m_quadShader.Use();
			m_quadShader.SetUniformInt("uTex", 0);
			m_quad.Draw();
		}
	}

	void HandleInput(const glm::mat4& viewProjection)
	{
		if (m_renderMode == RenderMode::Raster)
		{
			m_keyboardController.Update(m_time.GetDeltaTime());
		}

		if (m_keys.WasJustPressed(GLFW_KEY_ENTER))
		{
			if (m_renderMode == RenderMode::Raster)
			{
				m_renderer.Render(m_frameBuffer, m_camera, m_scene, viewProjection);
				m_renderMode = RenderMode::RayTracing;
			}
			else
			{
				m_renderMode = RenderMode::Raster;
			}
		}
	}

	void BuildScene()
	{
		PyramidScene::Build(m_scene);
		PyramidScene::Build(m_rasterScene);
		TeapotScene::Build(m_scene);
		TeapotScene::Build(m_rasterScene);
	}

	void InitCamera()
	{
		m_camera.SetPosition(glm::vec3{ 0, 1, 5 });
		m_camera.AddYaw(glm::radians<float>(-90));
	}

	void UpdateTitle()
	{
		std::string title = "Object scene. Mode: ";

		if (m_renderMode == RenderMode::Raster)
		{
			title += " Raster.";
		}
		else
		{
			title += " Ray Tracing.";
		}

		if (m_renderer.IsRendering())
		{
			title += " Rendering in progress...";
		}

		SetWindowTitle(title);
	}

	unsigned m_width;
	unsigned m_height;

	Scene m_scene;
	CompositeDrawable3D m_rasterScene;

	FirstPersonCamera m_camera;
	FirstPersonCameraController m_cameraController;
	KeyboardReader m_keys;
	KeyboardCameraController m_keyboardController;
	TimeProvider m_time;

	RenderMode m_renderMode = RenderMode::Raster;

	FrameBuffer m_frameBuffer;
	Renderer m_renderer;

	Quad m_quad;
	ShaderProgram m_quadShader;
	ShaderProgram m_rasterShader;
};