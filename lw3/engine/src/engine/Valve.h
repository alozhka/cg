#pragma once
#include "../shapes/Rectangle.hpp"
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class Valve : public CompositeObject
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
			Point{ 0, 0 },
			4,
			35,
			Palette::ValveColor);
		AddChild(body);

		auto plate = std::make_shared<Rectangle>(
			Point{ 0, -18 },
			16,
			5,
			Palette::ValveColor);
		AddChild(plate);
	}
};
} // namespace EngineParts