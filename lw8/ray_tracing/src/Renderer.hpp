#pragma once

#include "FrameBuffer.hpp"

#include <atomic>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <stop_token>
#include <thread>

class Renderer
{
public:
	Renderer() = default;

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	~Renderer()
	{
		Stop();
	}

	bool IsRendering() const
	{
		return m_rendering.load();
	}

	bool GetProgress(unsigned& renderedChunks, unsigned& totalChunks) const
	{
		std::scoped_lock lock(m_mutex);
		renderedChunks = m_renderedChunks.load();
		totalChunks = m_totalChunks.load();
		return (totalChunks > 0) && (renderedChunks == totalChunks);
	}

	bool Render(FrameBuffer& frameBuffer)
	{
		bool expected = false;
		if (!m_rendering.compare_exchange_strong(expected, true))
		{
			return false;
		}

		std::scoped_lock lock(m_mutex);

		frameBuffer.Clear();
		m_totalChunks.store(0);
		m_renderedChunks.store(0);

		m_thread = std::jthread(
			[this, &frameBuffer](const std::stop_token& st) {
				RenderFrame(st, frameBuffer);
			});
		return true;
	}

	void Stop()
	{
		if (m_thread.joinable())
		{
			m_thread.request_stop();
			m_thread.join();
		}
	}

private:
	void RenderFrame(const std::stop_token& stopToken, FrameBuffer& frameBuffer)
	{
		const int width = static_cast<int>(frameBuffer.GetWidth());
		const int height = static_cast<int>(frameBuffer.GetHeight());

		m_totalChunks.store(static_cast<std::uint32_t>(height));

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
		for (int y = 0; y < height; ++y)
		{
			if (stopToken.stop_requested())
			{
				continue;
			}

			std::uint32_t* rowPixels = frameBuffer.GetPixels(static_cast<unsigned>(y));
			for (int x = 0; x < width; ++x)
			{
				rowPixels[x] = CalculatePixelColor(x, y, width, height);
			}
			m_renderedChunks.fetch_add(1);
		}

		m_rendering.store(false);
	}

	// Заглушка: фрактал Мандельброта по формуле из README. Будет заменена
	// настоящей трассировкой лучей в Этапе 2.
	static std::uint32_t CalculatePixelColor(int x, int y, int width, int height)
	{
		const double x0 = 2.0 * x / width - 1.5;
		const double y0 = 2.0 * y / height - 1.0;

		const double rho = std::sqrt((x0 - 0.25) * (x0 - 0.25) + y0 * y0);
		const double theta = std::atan2(y0, x0 - 0.25);
		const double rhoC = 0.5 - 0.5 * std::cos(theta);
		if (rho <= rhoC)
		{
			return 0xff000000;
		}

		double re = 0;
		double im = 0;
		int iterCount = 10000;
		while ((iterCount > 0) && (re * re + im * im < 1e18))
		{
			const double re1 = re * re - im * im + x0;
			im = 2 * re * im + y0;
			re = re1;
			--iterCount;
		}

		std::uint8_t r = static_cast<std::uint8_t>((iterCount / 3) & 0xff);
		std::uint8_t g = static_cast<std::uint8_t>(iterCount & 0xff);
		std::uint8_t b = static_cast<std::uint8_t>((iterCount / 2) & 0xff);
		std::uint8_t a = 0xff;
		return static_cast<std::uint32_t>(a) << 24 | static_cast<std::uint32_t>(r) << 16 | static_cast<std::uint32_t>(g) << 8 | static_cast<std::uint32_t>(b);
	}

	mutable std::mutex m_mutex;
	std::atomic_bool m_rendering{ false };
	std::atomic_uint32_t m_totalChunks{ 0 };
	std::atomic_uint32_t m_renderedChunks{ 0 };
	std::jthread m_thread;
};