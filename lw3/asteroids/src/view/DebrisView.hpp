#pragma once

#include "../graphics/Color.hpp"
#include "../graphics/Mat3.hpp"
#include "../graphics/Mesh.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../model/Debris.hpp"

#include <memory>
#include <vector>

class DebrisView
{
public:
	void Render(ShaderProgram& shader, const Mat3& projection, const std::vector<Debris>& debrisList)
	{
		for (const auto& d : debrisList)
		{
			std::vector<float> lineData = {
				d.lineStart.x, d.lineStart.y,
				d.lineEnd.x, d.lineEnd.y,
			};
			Mesh lineMesh(lineData, GL_LINES);

			Mat3 model = Mat3::Translate(d.position.x, d.position.y)
				* Mat3::Rotate(d.angle);
			Mat3 mvp = projection * model;

			Color c{ 1.0f, 1.0f, 1.0f, d.GetAlpha() };

			shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
			shader.SetUniformVec4("uColor", c.ToFloatArray());
			lineMesh.Draw();
		}
	}
};
