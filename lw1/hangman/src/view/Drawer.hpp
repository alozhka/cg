#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Drawer
{
public:
	static void DrawLine(sf::RenderTarget& target, float x1, float y1, float x2, float y2, float thickness = 1.0f, sf::Color color = sf::Color::Black);
	static void drawCircle(sf::RenderTarget& target, float cx, float cy, float r, int segments, float thickness = 1.0f, sf::Color color = sf::Color::Black);
};
