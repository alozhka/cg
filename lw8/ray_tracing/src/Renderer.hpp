#pragma once

#include "FrameBuffer.hpp"

#include <atomic>
#include <cstdint>
#include <functional>
#include <mutex>
#include <stop_token>
#include <thread>

class Renderer
{
public:
	// Функция вычисления цвета одного пикселя. Получает координаты пикселя
	// и размеры буфера кадра.
	using PixelShader = std::function<std::uint32_t(int x, int y, int w, int h)>;

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

	bool Render(FrameBuffer& frameBuffer, const PixelShader& shader)
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
			[this, &frameBuffer, shader](const std::stop_token& st) {
				RenderFrame(st, frameBuffer, shader);
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
	void RenderFrame(const std::stop_token& stopToken,
		FrameBuffer& frameBuffer, const PixelShader& shader)
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
				rowPixels[x] = shader(x, y, width, height);
			}
			m_renderedChunks.fetch_add(1);
		}

		m_rendering.store(false);
	}

	mutable std::mutex m_mutex;
	std::atomic_bool m_rendering{ false };
	std::atomic_uint32_t m_totalChunks{ 0 };
	std::atomic_uint32_t m_renderedChunks{ 0 };
	std::jthread m_thread;
};