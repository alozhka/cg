#pragma once
#include "../graphics/CompositeObject.hpp"
#include "../shapes/Rectangle.hpp"

namespace EngineParts
{
class SparkPlug final : public CompositeObject
{
public:
	explicit SparkPlug()
	{
		Init();
	}

private:
	void Init()
	{
		auto plug = std::make_shared<Rectangle>(
			Vec2f{ 0, 20 },
			10,
			25,
			Palette::SparkPlug);
		AddChild(plug);

		auto electrode = std::make_shared<Rectangle>(
			Vec2f{ 0, 0 },
			3,
			15,
			Palette::DarkSteel);
		AddChild(electrode);
	}
};
} // namespace EngineParts