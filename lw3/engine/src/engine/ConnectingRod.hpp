#pragma once
#include "../shapes/Circle.hpp"
#include "../shapes/Rectangle.hpp"
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class ConnectingRod final : public CompositeObject
{

public:
	explicit ConnectingRod(double rodLength)
		: m_rodLength(rodLength)
	{
		Init();
	}

private:
	void Init()
	{
		auto rod = std::make_shared<Rectangle>(
			Point{ 10, m_rodLength / 2.0 },
			14,
			m_rodLength,
			Palette::DarkSteel,
			15);
		AddChild(rod);

		auto bottomConnectingRodHead = std::make_shared<Circle>(
			Point{ 0, m_rodLength },
			Palette::DarkSteel,
			16.0);
		AddChild(bottomConnectingRodHead);

		auto topConnectingRodHead = std::make_shared<Circle>(
			Point{ 0, 0 },
			Palette::DarkSteel,
			12.0);
		AddChild(topConnectingRodHead);
	}

	double m_rodLength;
};
} // namespace EngineParts