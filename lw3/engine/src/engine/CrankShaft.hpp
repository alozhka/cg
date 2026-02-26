#pragma once
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class CrankShaft final : public CompositeObject
{
public:
	explicit CrankShaft(double radius)
		: m_radius(radius)
	{
		Init();
	}

private:
	void Init()
	{
		auto counterWeight = std::make_shared<Circle>(
			Point{ 0, -20 },
			Palette::CrankGreen,
			45.0);
		AddChild(counterWeight);

		auto rotationAxis = std::make_shared<Circle>(
			Point{ 0, 0 },
			Palette::DarkSteel,
			10.0);
		AddChild(rotationAxis);

		auto connectingRodNeck = std::make_shared<Circle>(
			Point{ 0, m_radius },
			Palette::Steel,
			8.0);
		AddChild(connectingRodNeck);
	}

	double m_radius;
};
}; // namespace Engine