#pragma once
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class Piston : public CompositeObject
{
public:
	Piston(double pistonWidth, double pistonHeight)
		: m_pistonWidth(pistonWidth)
		, m_pistonHeight(pistonHeight)
	{
		Init();
	}

private:
	void Init()
	{
		auto mainBody = std::make_shared<Rectangle>(
			Point{ 0, 0 },
			m_pistonWidth,
			m_pistonHeight,
			Palette::Steel);
		AddChild(mainBody);

		auto topCompressionRing = std::make_shared<Rectangle>(
			Point{ 0, 10 },
			m_pistonWidth,
			3,
			Palette::DarkSteel);
		AddChild(topCompressionRing);

		auto bottomCompressionRing = std::make_shared<Rectangle>(
			Point{ 0, 4 },
			m_pistonWidth,
			3,
			Palette::DarkSteel);
		AddChild(bottomCompressionRing);

		auto pistonPin = std::make_shared<Circle>(
			Point{ 0, -5 },
			Palette::DarkSteel,
			8.0);
		AddChild(pistonPin);
	}

	double m_pistonWidth;
	double m_pistonHeight;
};
} // namespace EngineParts