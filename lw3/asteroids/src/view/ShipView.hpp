#pragma once

#include "../graphics/Color.hpp"
#include "../graphics/Mat3.hpp"
#include "../graphics/Mesh.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../model/Ship.hpp"

class ShipView
{
public:
	ShipView()
	{
		auto verts = Ship::GetShipVertices();
		std::vector<float> meshData;
		for (const auto& v : verts)
		{
			meshData.push_back(v.x);
			meshData.push_back(v.y);
		}
		m_shipMesh = std::make_unique<Mesh>(meshData, GL_LINE_LOOP);

		std::vector<float> flameData = {
			-Ship::SHIP_RADIUS * 0.3f, -Ship::SHIP_RADIUS * 0.4f,
			0.0f, -Ship::SHIP_RADIUS * 1.1f,
			Ship::SHIP_RADIUS * 0.3f, -Ship::SHIP_RADIUS * 0.4f,
		};
		m_flameMesh = std::make_unique<Mesh>(flameData, GL_LINE_LOOP);
	}

	void Render(ShaderProgram& shader, const Mat3& projection, const Ship& ship)
	{
		if (!ship.alive) return;

		Mat3 model = Mat3::Translate(ship.position.x, ship.position.y)
			* Mat3::Rotate(ship.angle);
		Mat3 mvp = projection * model;

		shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
		shader.SetUniformVec4("uColor", m_shipColor.ToFloatArray());
		m_shipMesh->Draw();

		if (ship.thrusting)
		{
			m_flameFlicker = !m_flameFlicker;
			if (m_flameFlicker)
			{
				shader.SetUniformVec4("uColor", m_flameColor.ToFloatArray());
				m_flameMesh->Draw();
			}
		}
	}

private:
	std::unique_ptr<Mesh> m_shipMesh;
	std::unique_ptr<Mesh> m_flameMesh;
	bool m_flameFlicker = false;

	Color m_shipColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Color m_flameColor{ 1.0f, 0.5f, 0.0f, 1.0f };
};
