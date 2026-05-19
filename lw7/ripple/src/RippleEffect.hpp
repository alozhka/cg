#pragma once

#include "QuadMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/Texture.hpp>

#include <glm/glm.hpp>

class RippleEffect
{
public:
	RippleEffect()
		: m_initialTexture("assets/heisenberg_stunned.jpg")
		, m_targetTexture("assets/heisenberg_lying.jpg")
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
	}

	void Draw(glm::vec2 rippleOrigin, float transitionTime, bool flipped)
	{
		m_shader.Use();

		if (flipped)
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
		m_shader.SetUniformVec2("uOrigin", rippleOrigin);
		m_shader.SetUniformFloat("uTime", transitionTime);

		m_quad.Draw();
	}

private:
	ShaderProgram m_shader;
	Texture m_initialTexture;
	Texture m_targetTexture;
	QuadMesh m_quad;
};
