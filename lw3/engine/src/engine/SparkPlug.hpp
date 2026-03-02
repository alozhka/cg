#pragma once
#include "../shapes/Rectangle.hpp"
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class SparkPlug : public CompositeObject
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
			Point{ 0, 20 },
			10,
			25,
			Palette::SparkPlug);
		AddChild(plug);

		auto electrode = std::make_shared<Rectangle>(
			Point{ 0, 0 },
			3,
			15,
			Palette::DarkSteel);
		AddChild(electrode);
	}
};
} // namespace EngineParts