#include "Drawer.hpp"
#include <cmath>
#include <numbers>
#include <vector>

void Drawer::DrawLine(sf::RenderTarget& target, float x1, float y1, float x2, float y2, float thickness, sf::Color color)
{
	float length = std::hypot(x2 - x1, y2 - y1);
	sf::RectangleShape line(sf::Vector2f(length, thickness));
	line.setOrigin(sf::Vector2f(0, thickness / 2.0f));
	line.setPosition(sf::Vector2f(x1, y1));
	line.setRotation(sf::degrees(std::atan2(y2 - y1, x2 - x1)) * 180.f / std::numbers::pi);
	line.setFillColor(color);
	target.draw(line);
}

void Drawer::drawCircle(sf::RenderTarget& target, float cx, float cy, float r, int segments, float thickness, sf::Color color)
{
	sf::CircleShape circle(r);
	circle.setPointCount(segments);
	circle.setPosition(sf::Vector2f(cx - r, cy - r));
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(thickness);
	circle.setOutlineColor(color);
	target.draw(circle);
}
