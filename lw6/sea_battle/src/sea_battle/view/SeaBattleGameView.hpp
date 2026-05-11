#pragma once
#include "../view_model/SeaBattleGameViewModel.hpp"
#include "ShipsView.hpp"

#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>

class SeaBattleGameView
{
public:
	SeaBattleGameView(const SeaBattleGameViewModel& gameViewModel, TextureCache& textureCache)
		: m_shipsView(gameViewModel.CreateShipsViewModel(), textureCache)
	{
	}

	void Draw(ShaderProgram& shader)
	{
		m_shipsView.Draw(shader);
	}

private:
	ShipsView m_shipsView;
};