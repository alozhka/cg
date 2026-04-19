#pragma once
#include "../model/MemoryTrainer.hpp"
#include "CardsViewModel.hpp"

class MemoryTrainerViewModel
{
public:
	explicit MemoryTrainerViewModel(const MemoryTrainerPtr& game)
		: m_cards(game)
	{
	}

	CardsViewModel& GetCardsViewModel()
	{
		return m_cards;
	}

	void TryPick(const glm::vec3& rayOrigin, const glm::vec3& rayDir)
	{
		m_cards.TryFlipAt(rayOrigin, rayDir);
	}

private:
	CardsViewModel m_cards;
};