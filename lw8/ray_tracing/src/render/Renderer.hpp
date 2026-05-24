#pragma once

#include "../core/Ray.hpp"
#include "../core/Scene.hpp"
#include "../shading/PhongShader.hpp"
#include "FrameBuffer.hpp"
#include "glm/ext/matrix_common.hpp"

#include <atomic>
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

	bool Render(FrameBuffer& frameBuffer, const FirstPersonCamera& camera, const Scene& scene, const glm::mat4& viewProjection)
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
			[this, &frameBuffer, &scene, camera, viewProjection](const std::stop_token& st) {
				RenderFrame(st, frameBuffer, scene, camera, viewProjection);
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
	void RenderFrame(
		const std::stop_token& stopToken,
		FrameBuffer& frameBuffer,
		const Scene& scene,
		const FirstPersonCamera& camera,
		const glm::mat4& viewProjection)
	{
		const int width = static_cast<int>(frameBuffer.GetWidth());
		const int height = static_cast<int>(frameBuffer.GetHeight());

		m_totalChunks.store(static_cast<uint32_t>(height));
		glm::mat4 invVP = glm::inverse(viewProjection);

#pragma omp parallel for schedule(dynamic)
		for (int y = 0; y < height; ++y)
		{
			if (stopToken.stop_requested())
			{
				continue;
			}

			uint32_t* rowPixels = frameBuffer.GetPixels(y);
			for (int x = 0; x < width; ++x)
			{
				Ray ray = CreatePrimaryRay(camera.GetPosition(), invVP, x, y, width, height);
				glm::vec3 color = TraceRay(scene, ray);
				rowPixels[x] = PackColor(color);
			}

			m_renderedChunks.fetch_add(1);
		}

		m_rendering.store(false);
	}

	static Ray CreatePrimaryRay(const glm::vec3& pos, const glm::mat4& invViewProjection, int x, int y, int w, int h)
	{
		float u = 2 * (static_cast<float>(x) + 0.5f) / static_cast<float>(w) - 1;
		float v = 1 - 2 * (static_cast<float>(y) + 0.5f) / static_cast<float>(h);

		glm::vec4 farClip{ u, v, 1, 1 };
		glm::vec4 farWorld = invViewProjection * farClip;
		farWorld /= farWorld.w;
		glm::vec3 direction = glm::normalize(glm::vec3(farWorld) - pos);

		return Ray{ pos, direction };
	}

	static glm::vec3 TraceRay(const Scene& scene, const Ray& ray)
	{
		HitInfo hit;
		if (!scene.Intersect(ray, hit))
		{
			return SkyGradient(ray);
		}

		return PhongShader::Shade(hit, -ray.direction, scene);
	}

	static uint32_t PackColor(glm::vec3 c)
	{
		c = glm::clamp(c, glm::vec3{ 0 }, glm::vec3{ 1 });

		auto r = static_cast<uint8_t>(c.r * 255);
		auto g = static_cast<uint8_t>(c.g * 255);
		auto b = static_cast<uint8_t>(c.b * 255);

		constexpr uint8_t a = 0xFF;

		return static_cast<uint32_t>(a) << 24
			| static_cast<uint32_t>(r) << 16
			| static_cast<uint32_t>(g) << 8
			| static_cast<uint32_t>(b);
	}

	static glm::vec3 SkyGradient(const Ray& ray)
	{
		const float t = 0.5f * (ray.direction.y + 1);
		return glm::mix(glm::vec3{ 1 }, glm::vec3{ 0.5, 0.7, 1 }, t);
	}

	mutable std::mutex m_mutex;
	std::atomic_bool m_rendering{ false };
	std::atomic_uint32_t m_totalChunks{ 0 };
	std::atomic_uint32_t m_renderedChunks{ 0 };
	std::jthread m_thread;
};