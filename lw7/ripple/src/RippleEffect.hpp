#pragma once

#include "QuadMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/Texture.hpp>

#include <glm/glm.hpp>

class RippleEffect
{
public:
	RippleEffect()
		: m_initialTexture("assets/initial_image.jpg") // heisenberg_stunned
		, m_targetTexture("assets/target_image.jpg") // heisenberg_lying
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
	}

	void Draw(glm::vec2 origin, float animTime, bool swapped)
	{
		m_shader.Use();

		if (swapped)
		{
			m_targetTexture.Bind(0);
			m_initialTexture.Bind(1);
		}
		else
		{
			m_initialTexture.Bind(0);
			m_targetTexture.Bind(1);
		}

		m_shader.SetUniformInt("uTex0", 0);
		m_shader.SetUniformInt("uTex1", 1);
		m_shader.SetUniformVec2("uOrigin", origin);
		m_shader.SetUniformFloat("uTime", animTime);

		m_quad.Draw();
	}

private:
	ShaderProgram m_shader;
	Texture m_initialTexture;
	Texture m_targetTexture;
	QuadMesh m_quad;
};
