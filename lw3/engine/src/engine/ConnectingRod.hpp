#pragma once
#include "../shared/CompositeObject.hpp"
#include "../shapes/Circle.hpp"

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
		AddChild(std::make_shared<Rectangle>(
			Point{ 0, -m_rodLength / 2.0 }, 14, m_rodLength,
			Palette::DarkSteel));

		auto bottomConnectingRodHead = std::make_shared<Circle>(
			Point{ 0, -m_rodLength },
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