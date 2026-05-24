#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer(unsigned width, unsigned height)
		: m_pixels(static_cast<size_t>(width) * height)
		, m_width(width)
		, m_height(height)
	{
	}

	unsigned GetWidth() const noexcept
	{
		return m_width;
	}

	unsigned GetHeight() const noexcept
	{
		return m_height;
	}

	void Clear(std::uint32_t color = 0)
	{
		std::fill(m_pixels.begin(), m_pixels.end(), color);
	}

	const std::uint32_t* GetPixels(unsigned row = 0) const noexcept
	{
		assert(row < m_height);
		return &m_pixels[static_cast<size_t>(row) * m_width];
	}

	std::uint32_t* GetPixels(unsigned row = 0) noexcept
	{
		assert(row < m_height);
		return &m_pixels[static_cast<size_t>(row) * m_width];
	}

	std::uint32_t GetPixel(unsigned x, unsigned y) const noexcept
	{
		assert(x < m_width);
		assert(y < m_height);
		return m_pixels[static_cast<size_t>(y) * m_width + x];
	}

	void SetPixel(unsigned x, unsigned y, std::uint32_t color) noexcept
	{
		assert(x < m_width);
		assert(y < m_height);
		m_pixels[static_cast<size_t>(y) * m_width + x] = color;
	}

private:
	std::vector<std::uint32_t> m_pixels;
	unsigned m_width;
	unsigned m_height;
};