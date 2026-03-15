#pragma once
#include "../model/AsteroidsGame.hpp"
#include "SpaceshipViewModel.hpp"

class AsteroidsGameViewModel
{
public:
	explicit AsteroidsGameViewModel(const AsteroidsGame& asteroidsGame)
		: m_asteroidsGame(asteroidsGame)
	{
	}

	SpaceshipViewModel GetSpaceshipViewModel() const
	{
		return SpaceshipViewModel(m_asteroidsGame.GetSpaceship());
	}

private:
	AsteroidsGame m_asteroidsGame;
};