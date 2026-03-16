#pragma once
#include <random>

class Randomizer
{
public:
	static float RandomRange(float min, float max)
	{
		std::uniform_real_distribution<float> dist{ min, max };
		return dist(m_rng);
	}

private:
	inline static std::mt19937 m_rng{ std::random_device{}() };
};