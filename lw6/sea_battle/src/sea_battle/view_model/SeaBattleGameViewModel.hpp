#pragma once
#include "../model/SeaBattleGame.hpp"
#include "ShipsViewModel.hpp"

class SeaBattleGameViewModel
{
public:
	explicit SeaBattleGameViewModel(SeaBattleGamePtr game)
		: m_game(std::move(game))
	{
	}

	ShipsViewModel CreateShipsViewModel() const
	{
		return ShipsViewModel(m_game);
	}

private:
	SeaBattleGamePtr m_game;
};