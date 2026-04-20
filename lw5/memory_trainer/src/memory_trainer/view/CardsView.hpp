#pragma once
#include "../view_model/CardsViewModel.hpp"

#include <graphics/TexturedMesh.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <string>

class CardsView
{
public:
	CardsView(CardsViewModel& viewModel, const std::string& texturesDir)
		: m_viewModel(viewModel)
		, m_mesh(CardsViewModel::GetCardVertices())
		, m_textures(m_viewModel.LoadTextures(texturesDir))
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection)
	{
		shader.Use();
		shader.SetUniformInt("uTexture", 0);

		for (const CardPlacement& placement : m_viewModel.GetPlacements())
		{
			if (placement.removed)
			{
				continue;
			}

			glm::mat4 model = GetTransformMatrix(placement);

			shader.SetUniformMat4("uModel", model);
			shader.SetUniformMat4("uViewProjection", viewProjection);

			m_textures.back.Bind(0);
			m_mesh.Draw(GL_TRIANGLES,
				CardsViewModel::BACK_TEXTURE_VERTEX_OFFSET,
				CardsViewModel::BACK_TEXTURE_VERTEX_COUNT);

			m_textures.fronts.at(placement.frontTexture).Bind(0);
			m_mesh.Draw(GL_TRIANGLES,
				CardsViewModel::FRONT_TEXTURE_VERTEX_OFFSET,
				CardsViewModel::FRONT_TEXTURE_VERTEX_COUNT);
		}
	}

private:
	static glm::mat4 GetTransformMatrix(const CardPlacement& placement)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, placement.gridPos.x, placement.gridPos.y));
		return glm::rotate(model, glm::radians(placement.angle), glm::vec3(0, 0, 1));
	}

	CardsViewModel& m_viewModel;
	TexturedMesh m_mesh;
	CardTextures m_textures;
};