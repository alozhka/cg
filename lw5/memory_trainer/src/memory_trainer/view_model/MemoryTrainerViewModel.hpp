#pragma once
#include "../model/MemoryTrainer.hpp"
#include "CardsViewModel.hpp"

class MemoryTrainerViewModel
{
public:
	explicit MemoryTrainerViewModel(const MemoryTrainerPtr& game)
		: m_game(game)
	{
	}

	CardsViewModel CreateCardsViewModel()
	{
		return CardsViewModel(m_game);
	}

private:
	MemoryTrainerPtr m_game;
};