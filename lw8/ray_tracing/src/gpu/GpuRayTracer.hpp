#pragma once

#include "../objects/Quad.hpp"

#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

class GpuRayTracer
{
public:
	GpuRayTracer()
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/raytrace_fragment.glsl");
	}

	void Render(const FirstPersonCamera& camera, const glm::mat4& viewProjection, Quad& quad)
	{
		m_shader.SetUniformVec3("uCameraPos", camera.GetPosition());
		m_shader.SetUniformMat4("uInvViewProjection", glm::inverse(viewProjection));
		m_shader.Use();
		quad.Draw();
	}

private:
	ShaderProgram m_shader;
};
