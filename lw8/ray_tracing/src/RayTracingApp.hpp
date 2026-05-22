#pragma once

#include "FrameBuffer.hpp"
#include "Renderer.hpp"
#include "Shading.hpp"

#include "core/Ray.hpp"
#include "scene/Plane.hpp"
#include "scene/Scene.hpp"
#include "scene/Sphere.hpp"

#include <graphics/TexturedMesh.hpp>
#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/FirstPersonCameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/KeyboardReader.hpp>
#include <graphics/windows/TimeProvider.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <memory>
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
		, m_cameraController(m_camera)
		, m_keys(CreateKeyboardReader())
		, m_quad(BuildQuadVertices())
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		InitTexture();
		// Изначально буфер обнулён — заливаем чёрную текстуру, чтобы до
		// первой презентации не было неинициализированного содержимого.
		UploadFrameBuffer();
		BuildScene();
		InitCamera();
		CaptureMouseInput();

		StartRender();
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
	void OnDraw(const glm::mat4&) override
	{
		HandleInput(m_time.GetDeltaTime());
		// Сначала презентуем готовый кадр (если есть), потом думаем о рестарте.
		// Иначе следующий StartRender перебьёт флаг и кадр уйдёт в небытие.
		MaybePresentFrame();
		MaybeRestartRender();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		m_shader.SetUniformInt("uTex", 0);

		m_quad.Draw(GL_TRIANGLES);
	}

	void OnRawMouseMove(double x, double y) override
	{
		m_cameraController.OnRawMouseMove(x, y);
	}

private:
	static constexpr float MOVE_SPEED = 3.0f;

	void BuildScene()
	{
		m_scene.Add(std::make_unique<Plane>(
			glm::vec3{ 0.f, 0.f, 0.f },
			glm::vec3{ 0.f, 1.f, 0.f },
			glm::vec3{ 0.6f, 0.6f, 0.6f }));

		m_scene.Add(std::make_unique<Sphere>(
			glm::vec3{ -1.2f, 0.5f, 0.f }, 0.5f, glm::vec3{ 0.9f, 0.2f, 0.2f }));
		m_scene.Add(std::make_unique<Sphere>(
			glm::vec3{ 0.f, 0.5f, 0.f }, 0.5f, glm::vec3{ 0.2f, 0.9f, 0.2f }));
		m_scene.Add(std::make_unique<Sphere>(
			glm::vec3{ 1.2f, 0.5f, 0.f }, 0.5f, glm::vec3{ 0.2f, 0.4f, 0.9f }));
	}

	void InitCamera()
	{
		m_camera.SetPosition(glm::vec3{ 0.f, 1.0f, 5.f });
		// yaw=0 смотрит вдоль +X. Поворачиваем на -90°, чтобы смотреть в -Z
		// (в направлении сцены).
		m_camera.AddYaw(glm::radians(-90.f));
	}

	void HandleInput(float dt)
	{
		const glm::vec3 forward = m_camera.GetForwardXZ();
		const glm::vec3 right = m_camera.GetRight();
		const float step = MOVE_SPEED * dt;

		glm::vec3 pos = m_camera.GetPosition();
		if (m_keys.IsButtonPressed(GLFW_KEY_W)) pos += forward * step;
		if (m_keys.IsButtonPressed(GLFW_KEY_S)) pos -= forward * step;
		if (m_keys.IsButtonPressed(GLFW_KEY_D)) pos += right * step;
		if (m_keys.IsButtonPressed(GLFW_KEY_A)) pos -= right * step;
		if (m_keys.IsButtonPressed(GLFW_KEY_SPACE)) pos.y += step;
		if (m_keys.IsButtonPressed(GLFW_KEY_LEFT_SHIFT)) pos.y -= step;

		m_camera.SetPosition(pos);
	}

	void MaybeRestartRender()
	{
		// Пока текущий кадр не дорендерился — не дёргаем рестарт, иначе экран
		// будет мигать чёрным/частично-отрисованным при каждом движении.
		if (m_renderer.IsRendering())
		{
			return;
		}

		const glm::vec3 pos = m_camera.GetPosition();
		const glm::vec3 fwd = m_camera.GetForward();
		if (pos == m_renderedPosition && fwd == m_renderedForward)
		{
			return;
		}

		StartRender();
	}

	void MaybePresentFrame()
	{
		if (m_renderer.IsRendering() || !m_framePending)
		{
			return;
		}
		UploadFrameBuffer();
		m_framePending = false;
	}

	void StartRender()
	{
		m_renderedPosition = m_camera.GetPosition();
		m_renderedForward = m_camera.GetForward();
		m_framePending = true;

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
				return PackColor(TraceRay(scene, ray));
			});
	}

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

	Scene m_scene;
	FirstPersonCamera m_camera;
	FirstPersonCameraController m_cameraController;
	KeyboardReader m_keys;
	TimeProvider m_time;

	// Состояние камеры на момент старта текущего/последнего рендера. Сравниваем
	// с актуальным состоянием, чтобы понять, нужно ли запускать новый кадр.
	glm::vec3 m_renderedPosition{ 0.f };
	glm::vec3 m_renderedForward{ 0.f };
	// Кадр построен, но ещё не залит в GL-текстуру.
	bool m_framePending = false;

	FrameBuffer m_frameBuffer;
	Renderer m_renderer;

	TexturedMesh m_quad;
	ShaderProgram m_shader;
	GLuint m_texture = 0;
};