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
struct DeleteButtonColorScheme
{
	sf::Color Fill;
	sf::Color Outline;
	sf::Color Icon;
};

constexpr DeleteButtonColorScheme DeleteButtonNormal{
	.Fill = { 230, 230, 230 },
	.Outline = { 180, 180, 180 },
	.Icon = { 150, 150, 150 },
};

constexpr DeleteButtonColorScheme DeleteButtonHovered{
	.Fill = { 255, 200, 200 },
	.Outline = { 220, 80, 80 },
	.Icon = { 200, 50, 50 },
};
} // namespace Colors