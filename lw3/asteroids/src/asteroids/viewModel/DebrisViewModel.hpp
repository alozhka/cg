#pragma once
#include "../../graphics/Vec2f.hpp"
#include "../model/AsteroidsGame.hpp"

#include <vector>

struct DebrisDto
{
	Vec2f position;
	float angle;
};

class DebrisViewModel
{
public:
	explicit DebrisViewModel(const AsteroidsGamePtr& asteroidsGame)
		: m_asteroidsGame(asteroidsGame)
	{
	}

	std::vector<DebrisDto> ListDebris() const
	{
		std::vector<Debris> debris = m_asteroidsGame->ListDebris();
		std::vector<DebrisDto> debrisDtos;
		debrisDtos.reserve(debris.size());

		for (const Debris& d : debris)
		{
			debrisDtos.push_back({ d.GetPosition(), d.GetAngle() });
		}

		return debrisDtos;
	}

private:
	AsteroidsGamePtr m_asteroidsGame;
};
