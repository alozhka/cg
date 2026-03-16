#pragma once
#include "../../graphics/Vec2f.hpp"
#include "Coordinates.hpp"
#include "Randomizer.hpp"

#include <vector>

enum class AsteroidSize
{
	Small = 0,
	Medium = 1,
	Large = 2
};

class Asteroid
{
public:
	Asteroid(AsteroidSize size, Vec2f position)
		: m_position(position)
		, m_vertices(CreateVertices(size))
	{
	}

	void Update(float dt, float width, float height)
	{
		m_position += m_velocity * dt;
		m_position = Coordinates::Wrap(m_position, width, height);
		m_angle += m_angularVelocity * dt;
		m_angle = Coordinates::WrapAngle(m_angle);
	}

	Vec2f GetPosition() const
	{
		return m_position;
	}

	float GetAngle() const
	{
		return m_angle;
	}

	std::vector<Vec2f> ListVertices() const
	{
		return m_vertices;
	}

private:
	static std::vector<Vec2f> CreateVertices(AsteroidSize size)
	{
		std::vector<Vec2f> vertices;
		vertices.reserve(AMOUNT_VERTICES);
		float scale = ScaleForAsteroidSize(size);

		for (int i = 0; i < AMOUNT_VERTICES; i++)
		{
			float angle = 360 / AMOUNT_VERTICES * i * std::numbers::pi_v<float> / 180;
			float radius = scale * (0.8 + Randomizer::RandomRange(0, 0.8));
			vertices.push_back({ radius * std::cos(angle), radius * std::sin(angle) });
		}

		return vertices;
	}

	static float ScaleForAsteroidSize(AsteroidSize size)
	{
		switch (size)
		{
		case AsteroidSize::Small:
			return 15;
		case AsteroidSize::Medium:
			return 22;
		case AsteroidSize::Large:
			return 30;
		default:
			throw std::invalid_argument("Unsupported asteroid size");
		}
	}

	static constexpr unsigned int AMOUNT_VERTICES = 8;

	Vec2f m_position;
	Vec2f m_velocity{Randomizer::RandomRange(-30, 30), Randomizer::RandomRange(-30, 30)};
	float m_angle = 0;
	float m_angularVelocity = Randomizer::RandomRange(-30, 30);
	std::vector<Vec2f> m_vertices;
};