#pragma once

struct Color
{
	double r = 0, g = 0, b = 0, a = 1;
};

namespace Palette
{
constexpr Color CastIron{ 0.70, 0.55, 0.45 };
constexpr Color CastIronDark{ 0.55, 0.40, 0.30 };
constexpr Color CylinderInner{ 0.85, 0.90, 0.95 };
constexpr Color Steel{ 0.75, 0.75, 0.80 };
constexpr Color DarkSteel{ 0.50, 0.50, 0.55 };
constexpr Color CrankGreen{ 0.60, 0.70, 0.60 };
constexpr Color ValveColor{ 0.40, 0.30, 0.20 };
constexpr Color SparkPlug{ 0.90, 0.90, 0.80 };
} // namespace Palette
