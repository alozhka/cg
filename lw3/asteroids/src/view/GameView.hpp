#pragma once

#include "../viewmodel/GameViewModel.hpp"
#include "AsteroidView.hpp"
#include "BulletView.hpp"
#include "DebrisView.hpp"
#include "ShipView.hpp"

class GameView
{
public:
	void Render(ShaderProgram& shader, const Mat3& projection, const GameViewModel& vm)
	{
		m_asteroidView.Render(shader, projection, vm.GetAsteroids());
		m_bulletView.Render(shader, projection, vm.GetBullets());
		m_shipView.Render(shader, projection, vm.GetShip());
		m_debrisView.Render(shader, projection, vm.GetDebris());
	}

	void OnRestart()
	{
		m_asteroidView.ClearMeshes();
	}

private:
	ShipView m_shipView;
	AsteroidView m_asteroidView;
	BulletView m_bulletView;
	DebrisView m_debrisView;
};
