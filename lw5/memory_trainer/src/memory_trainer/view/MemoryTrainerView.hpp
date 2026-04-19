#pragma once
#include "../view_model/MemoryTrainerViewModel.hpp"
#include "CardsView.hpp"
#include "graphics/shaders/ShaderProgram.hpp"

class MemoryTrainerView
{
public:
	MemoryTrainerView(MemoryTrainerViewModel& viewModel)
		: m_cards(viewModel.CreateCardsViewModel(), "assets/textures")
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& projection)
	{
		m_cards.Draw(shader, projection);
	}

private:
	CardsView m_cards;
};