#pragma once

#include "Drawable3D.hpp"
#include "Model.hpp"

#include <glm/glm.hpp>

class DrawableModel : public Drawable3D
{
public:
	DrawableModel() = default;

	explicit DrawableModel(Model& model)
		: m_model(&model)
	{
	}

	DrawableModel(
		Model& model,
		const glm::vec3& pos,
		const glm::vec3& rotationDegrees = glm::vec3(0),
		const glm::vec3& scale = glm::vec3(1))
		: Drawable3D(pos, rotationDegrees, scale)
		, m_model(&model)
	{
	}

	void SetModel(Model& model)
	{
		m_model = &model;
	}

	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		if (!m_model)
		{
			return;
		}

		const glm::mat4 modelTransform = parentTransform * GetTransformMatrix();
		SetModelUniforms(shader, modelTransform);
		m_model->Draw(shader);
	}

private:
	static void SetModelUniforms(ShaderProgram& shader, const glm::mat4& modelTransform)
	{
		shader.SetUniformMat4("uModel", modelTransform);
		shader.SetUniformMat3("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(modelTransform))));
	}

	Model* m_model = nullptr;
};
