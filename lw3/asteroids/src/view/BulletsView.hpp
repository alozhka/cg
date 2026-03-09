#pragma once

#include "../graphics/Color.hpp"
#include "../graphics/Mat3.hpp"
#include "../graphics/Mesh.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../viewmodel/ViewData.hpp"

#include <memory>
#include <vector>

class BulletsView
{
public:
	BulletsView()
	{
		constexpr float s = 2.0f;
		std::vector<float> data = {
			-s, -s,
			 s, -s,
			 s,  s,
			-s, -s,
			 s,  s,
			-s,  s,
		};
		m_mesh = std::make_unique<Mesh>(data, GL_TRIANGLES);
	}

	void Render(ShaderProgram& shader, const Mat3& projection, const std::vector<BulletViewModel>& bullets)
	{
		shader.SetUniformVec4("uColor", m_color.ToFloatArray());

		for (const auto& b : bullets)
		{
			Mat3 model = Mat3::Translate(b.position.x, b.position.y);
			Mat3 mvp = projection * model;
			shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
			m_mesh->Draw();
		}
	}

private:
	std::unique_ptr<Mesh> m_mesh;
	Color m_color{ 1.0f, 1.0f, 0.5f, 1.0f };
};
