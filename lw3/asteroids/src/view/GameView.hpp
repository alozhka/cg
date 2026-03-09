#pragma once

#include "../viewmodel/GameViewModel.hpp"
#include "AsteroidsView.hpp"
#include "BulletsView.hpp"
#include "DebrisView.hpp"
#include "ShipView.hpp"

class GameView
{
public:
	void Render(ShaderProgram& shader, const Mat3& projection, const GameViewModel& vm)
	{
		m_asteroidsView.Render(shader, projection, vm.GetAsteroidsViewModel());
		m_bulletsView.Render(shader, projection, vm.GetBulletsViewModel());
		m_shipView.Render(shader, projection, vm.GetShipViewModel());
		m_debrisView.Render(shader, projection, vm.GetDebrisViewModel());
	}

	void OnRestart()
	{
		m_asteroidsView.ClearMeshes();
	}

private:
	ShipView m_shipView;
	AsteroidsView m_asteroidsView;
	BulletsView m_bulletsView;
	DebrisView m_debrisView;
};
