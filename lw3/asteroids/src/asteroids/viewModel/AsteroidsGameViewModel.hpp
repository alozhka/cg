#pragma once
#include "../model/AsteroidsGame.hpp"
#include "BulletsViewModel.hpp"
#include "SpaceshipViewModel.hpp"

class AsteroidsGameViewModel
{
public:
	explicit AsteroidsGameViewModel(const AsteroidsGamePtr& asteroidsGame)
		: m_asteroidsGame(asteroidsGame)
	{
	}

	SpaceshipViewModel CreateSpaceshipViewModel() const
	{
		return SpaceshipViewModel(m_asteroidsGame->GetSpaceship());
	}

	BulletsViewModel CreateBulletsViewModel() const
	{
		return BulletsViewModel(m_asteroidsGame);
	}

private:
	AsteroidsGamePtr m_asteroidsGame;
};