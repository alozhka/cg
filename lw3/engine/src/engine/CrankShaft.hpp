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

	}

	double m_radius;
};
} // namespace EngineParts