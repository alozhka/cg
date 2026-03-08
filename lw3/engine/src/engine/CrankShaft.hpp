#pragma once
#include "../shared/CompositeObject.hpp"

namespace EngineParts
{
class CrankShaft final : public CompositeObject
{
public:
	explicit CrankShaft(float radius)
		: m_radius(radius)
	{
		Init();
	}

private:
	void Init()
	{

	}

	float m_radius;
};
} // namespace EngineParts