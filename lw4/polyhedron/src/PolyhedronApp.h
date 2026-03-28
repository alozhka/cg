#pragma once
#include "Cube.hpp"
#include "graphics/light/DirectLight.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class PolyhedronApp final : public GraphicsApplication
{
public:
	explicit PolyhedronApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_cameraController(m_camera)
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		m_cube.SetScale(5);

		DirectLight light({ 1, 1, 3 });
		light.SetDiffuseIntensity(0.5, 0.5, 0.5);
		light.SetAmbientIntensity(0.2, 0.2, 0.2);
		light.SetSpecularIntensity(0.3, 0.3, 0.3);
		light.SetLight(GL_LIGHT0);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
	}

protected:
	void OnDraw(const glm::mat4& projection) override
	{
		glClearColor(0.08, 0.08, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		GLfloat specularColor[4] = { 0.3, 0.3, 0.3, 1 };
		glMaterialf(GL_FRONT, GL_SPECULAR, *specularColor);
		glMaterialf(GL_FRONT, GL_SHININESS, 20);

		glm::mat4 mvp = projection * m_camera.GetViewMatrix();
		m_shader.Use();

		m_cube.Draw(m_shader, mvp);
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
	OrbitalCamera m_camera;
	CameraController m_cameraController;
	ShaderProgram m_shader;

	Cube m_cube{};
};