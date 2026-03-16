#pragma once
#include "../../graphics/Mat3.hpp"
#include "../../graphics/ShaderProgram.hpp"
#include "../viewModel/AsteroidsGameViewModel.hpp"
#include "BulletsView.h"
#include "SpaceshipView.hpp"

class AsteroidsGameView
{
public:
	explicit AsteroidsGameView(const AsteroidsGameViewModel& gameViewModel)
		: m_spaceshipView(gameViewModel.CreateSpaceshipViewModel())
		, m_bulletsView(gameViewModel.CreateBulletsViewModel())
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		m_spaceshipView.Draw(shader, projection);
		m_bulletsView.Draw(shader, projection);
	}

private:
	SpaceshipView m_spaceshipView;
	BulletsView m_bulletsView;
};