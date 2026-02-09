#pragma once
#include <vector>
#include <optional>

class AlphabetView
{
public:
	AlphabetView(float startX, float startY, float boxSize, float padding);

	void draw(const std::vector<int>& alphabetStates) const;
	std::optional<char> getLetterAt(double x, double y) const;

private:
	float m_startX;
	float m_startY;
	float m_boxSize;
	float m_padding;
	int m_cols = 13;
};
