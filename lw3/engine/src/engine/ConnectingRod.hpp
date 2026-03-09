#pragma once
#include "../graphics/CompositeDrawable.hpp"
#include "../shapes/Circle.hpp"
#include "../shapes/Rectangle.hpp"

namespace EngineParts
{
class ConnectingRod final : public CompositeDrawable
{

public:
	explicit ConnectingRod(float rodLength)
		: m_rodLength(rodLength)
	{
		Init();
	}

private:
	void Init()
	{
		auto rod = std::make_shared<Rectangle>(
			Vec2f{ 0, m_rodLength / 2 },
			14,
			m_rodLength,
			Palette::DarkSteel);
		AddChild(rod);

		auto bottomConnectingRodHead = std::make_shared<Circle>(
			Vec2f{ 0, m_rodLength },
			Palette::DarkSteel,
			16.0);
		AddChild(bottomConnectingRodHead);

		auto topConnectingRodHead = std::make_shared<Circle>(
			Vec2f{ 0, 0 },
			Palette::DarkSteel,
			12.0);
		AddChild(topConnectingRodHead);
	}

	float m_rodLength;
};
} // namespace EngineParts