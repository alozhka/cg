#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

class AlphabetView
{
public:
	AlphabetView(const sf::Font& font, float startX, float startY, float boxSize, float padding);

	void draw(sf::RenderTarget& target, const std::vector<int>& alphabetStates) const;

	std::optional<char> getLetterAt(double mouseX, double mouseY) const;

private:
	const sf::Font& m_font;
	float m_startX;
	float m_startY;
	float m_boxSize;
	float m_padding;
	int m_cols = 13;
};
