#pragma once
#include "../graphics/CompositeObject.hpp"
#include "../shapes/Circle.hpp"
#include "../shapes/Rectangle.hpp"

namespace EngineParts
{
class CrankShaft final : public CompositeObject
{
public:
	explicit CrankShaft(float radius)
		: m_radius(radius)
	{
		Init();
	}

private:
	void Init()
	{
		// Crank web (щека)
		auto crankWeb = std::make_shared<Rectangle>(
			Vec2f{ 0, m_radius / 2 - 10 },
			20,
			m_radius + 20,
			Palette::CrankGreen);
		AddChild(crankWeb);

		// Main journal (коренная шейка)
		auto mainJournal = std::make_shared<Circle>(
			Vec2f{ 0, 0 },
			Palette::DarkSteel,
			16.0);
		AddChild(mainJournal);

		// Crank pin (шатунная шейка)
		auto crankPin = std::make_shared<Circle>(
			Vec2f{ 0, m_radius },
			Palette::Steel,
			12.0);
		AddChild(crankPin);
	}

	float m_radius;
};
} // namespace EngineParts