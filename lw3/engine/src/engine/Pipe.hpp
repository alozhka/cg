#pragma once
#include "../shapes/Rectangle.hpp"
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class Pipe final : public CompositeObject
{
public:
	explicit Pipe(double degrees)
		: m_degrees(degrees)
	{
		Init();
	}

private:
	void Init()
	{
		auto pipeBody = std::make_shared<Rectangle>(
			Point{ 0, 0 },
			60,
			15,
			Palette::CastIron,
			m_degrees);
		AddChild(pipeBody);

		auto pipeInside = std::make_shared<Rectangle>(
			Point{ 0, 0 },
			60,
			8,
			Palette::CylinderInner,
			m_degrees);
		AddChild(pipeInside);
	}

	double m_degrees;
};
} // namespace EngineParts