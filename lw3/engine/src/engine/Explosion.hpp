#pragma once
#include "../graphics/Color.hpp"
#include "../graphics/Drawable.hpp"
#include "../graphics/Mesh.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

class Explosion final : public Drawable
{
public:
	explicit Explosion(float maxRadius, float duration)
		: m_maxRadius(maxRadius)
		, m_duration(duration)
		, m_mesh(CreateCircleMesh(maxRadius))
	{
	}

	void Trigger()
	{
		if (!m_active)
		{
			m_active = true;
			m_time = 0;
		}
	}

	void Update(float dt) override
	{
		if (!m_active)
		{
			return;
		}

		m_time += dt;
		if (m_time >= m_duration)
		{
			m_active = false;
		}
	}

	void Draw(ShaderProgram& shader, const Mat3& parentTransform) override
	{
		if (!m_active)
		{
			return;
		}

		float t = std::clamp(m_time / m_duration, 0.0f, 1.0f);

		float scale = 0.2f + 0.8f * t;
		SetScale(scale);

		Mat3 world = parentTransform * GetTransformMatrix();
		shader.SetUniformMat3("uViewProjection", world.ToFloatArray());
		shader.SetUniformVec4("uColor", Palette::Explosion.ToFloatArray());

		m_mesh.Draw();
	}

private:
	static Mesh CreateCircleMesh(float radius, int segments = 30)
	{
		std::vector<float> vertices;
		vertices.reserve((segments + 2) * 2);
		vertices.push_back(0);
		vertices.push_back(0);

		for (int i = 0; i <= segments; i++)
		{
			float theta = 2 * std::numbers::pi_v<float> * i / segments;
			vertices.push_back(radius * std::cos(theta));
			vertices.push_back(radius * std::sin(theta));
		}

		return Mesh{ vertices, GL_TRIANGLE_FAN };
	}

	float m_maxRadius;
	float m_duration;
	float m_time = 0;
	bool m_active = false;
	Mesh m_mesh;
};
