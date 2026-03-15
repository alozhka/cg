#pragma once
#include "../../graphics/Mat3.hpp"
#include "../../graphics/ShaderProgram.hpp"
#include "../viewModel/AsteroidsGameViewModel.hpp"
#include "SpaceshipView.hpp"

class AsteroidsGameView
{
public:
	AsteroidsGameView(const AsteroidsGameViewModel& gameViewModel)
		: m_spaceshipView(gameViewModel.GetSpaceshipViewModel())
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		m_spaceshipView.Draw(shader, projection);
	}

private:
	SpaceshipView m_spaceshipView;
};