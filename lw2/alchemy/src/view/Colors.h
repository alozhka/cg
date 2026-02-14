#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Colors
{
constexpr sf::Color LightGray{ 240, 240, 240 };

struct CardColorScheme
{
	sf::Color CardFill;
	sf::Color CardOutline;
	sf::Color PlaceholderFill;
	sf::Color PlaceholderOutline;
	sf::Color PlaceholderText;
	sf::Color LabelText;
};

constexpr CardColorScheme DiscoveredCard{
	.CardFill = { 220, 235, 255 },
	.CardOutline = { 100, 150, 220 },
	.PlaceholderFill = { 200, 215, 240 },
	.PlaceholderOutline = { 140, 170, 210 },
	.PlaceholderText = { 100, 140, 200 },
	.LabelText = { 30, 30, 30 },
};

constexpr CardColorScheme UndiscoveredCard{
	.CardFill = { 215, 215, 215 },
	.CardOutline = { 180, 180, 180 },
	.PlaceholderFill = { 195, 195, 195 },
	.PlaceholderOutline = { 165, 165, 165 },
	.PlaceholderText = { 150, 150, 150 },
	.LabelText = { 140, 140, 140 },
};
} // namespace Colors