#pragma once
#include "../shapes/Rectangle.hpp"
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class Valve final : public CompositeObject
{
public:
	Valve()
	{
		Init();
	}

private:
	void Init()
	{
		auto body = std::make_shared<Rectangle>(
			Vec2f{ 0, 0 },
			4,
			35,
			Palette::ValveColor);
		AddChild(body);

		auto plate = std::make_shared<Rectangle>(
			Vec2f{ 0, -18 },
			16,
			5,
			Palette::ValveColor);
		AddChild(plate);
	}
};
} // namespace EngineParts