#pragma once

#include "SurfaceMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>

#include <glm/glm.hpp>

#include <cmath>
#include <string>

class MobiusToKleinMorphing
{
public:
	MobiusToKleinMorphing()
		: m_mesh(GridU, GridV)
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
	}

	void Draw(const glm::mat4& mvp, float time)
	{
		const float phase = 0.5 * (1 - std::cos(time * MorphSpeed));

		m_shader.Use();
		m_shader.SetUniformMat4("uMVP", mvp);
		m_shader.SetUniformFloat("uPhase", phase);
		m_shader.SetUniformFloat("uR", BigRadius);
		m_shader.SetUniformFloat("uMobiusHalfWidth", MobiusHalfWidth);
		m_shader.SetUniformVec3("uColor", glm::vec3(0.35, 0.95, 0.55));
		m_shader.SetUniformVec3("uLightDir", glm::normalize(glm::vec3(1.0, 0.4, 0.5)));

		m_mesh.Draw();
	}

private:
	static constexpr int GridU = 220;
	static constexpr int GridV = 60;
	static constexpr float BigRadius = 1;
	static constexpr float MobiusHalfWidth = 0.4;
	static constexpr float MorphSpeed = 1.2;

	ShaderProgram m_shader;
	SurfaceMesh m_mesh;
};
