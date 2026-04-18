#pragma once
#include <string>

class Card
{
public:
	Card(const std::string& name, size_t row, size_t col)
		: m_name(name)
		, m_row(row)
		, m_col(col)
	{
	}

private:
	std::string m_name;
	size_t m_row, m_col;
};