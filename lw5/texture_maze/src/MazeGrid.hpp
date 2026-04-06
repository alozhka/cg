#pragma once

#include <array>
#include <cmath>

template<int N>
class MazeGrid
{
public:

	constexpr explicit MazeGrid(const std::array<std::array<int, N>, N>& grid)
		: m_grid(grid)
	{
	}

	bool IsWall(float x, float z) const
	{
		int col = static_cast<int>(std::floor(x));
		int row = static_cast<int>(std::floor(z));

		if (IsOutOfBounds(row, col))
		{
			return true;
		}

		return m_grid[row][col] == 1;
	}

	bool IsOpen(int row, int col) const
	{
		if (IsOutOfBounds(row, col))
		{
			return false;
		}

		return m_grid[row][col] == 0;
	}

private:
	static bool IsOutOfBounds(int row, int col)
	{
		return row < 0 || row >= N || col < 0 || col >= N;
	}

	std::array<std::array<int, N>, N> m_grid;
};