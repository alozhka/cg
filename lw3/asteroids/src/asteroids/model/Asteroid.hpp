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
		: m_size(size)
		, m_position(position)
		, m_vertices(CreateVertices(size))
	{
	}

	void Update(float dt, float width, float height)
	{
		m_position += m_velocity * dt;
		m_position = Coordinates::Wrap(m_position, width, height, RadiusForAsteroidSize(m_size));
		m_angle += m_angularVelocity * dt;
		m_angle = Coordinates::WrapAngle(m_angle);
	}

	std::vector<Asteroid> Split() const
	{
		if (m_size == AsteroidSize::Small)
		{
			return {};
		}

		AsteroidSize lowerSize = LowerAsteroidSize(m_size);
		std::vector<Asteroid> children;
		for (int i = 0; i < SPLIT_ASTEROIDS_AMOUNT; ++i)
		{
			children.emplace_back(lowerSize, m_position);
		}

		return children;
	}

	Vec2f GetPosition() const
	{
		return m_position;
	}

	float GetAngle() const
	{
		return m_angle;
	}

	AsteroidSize GetSize() const
	{
		return m_size;
	}

	std::vector<Vec2f> ListVertices() const
	{
		return m_vertices;
	}

	std::vector<Vec2f> ListWorldVertices() const
	{
		std::vector<Vec2f> world;
		world.reserve(m_vertices.size());
		Mat3 transform = Mat3::Translate(m_position.x, m_position.y) * Mat3::Rotate(m_angle);

		for (const Vec2f& v : m_vertices)
		{
			world.push_back(transform * v);
		}

		return world;
	}

private:
	static std::vector<Vec2f> CreateVertices(AsteroidSize size)
	{
		std::vector<Vec2f> vertices;
		vertices.reserve(AMOUNT_VERTICES);

		for (int i = 0; i < AMOUNT_VERTICES; i++)
		{
			float angle = 360 / AMOUNT_VERTICES * i * std::numbers::pi_v<float> / 180;
			float radius = RadiusForAsteroidSize(size) * (0.8 + Randomizer::RandomRange(0, 0.8));
			vertices.push_back({ radius * std::cos(angle), radius * std::sin(angle) });
		}

		return vertices;
	}

	static float RadiusForAsteroidSize(AsteroidSize size)
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

	static AsteroidSize LowerAsteroidSize(AsteroidSize size)
	{
		switch (size)
		{
		case AsteroidSize::Large:
			return AsteroidSize::Medium;
		case AsteroidSize::Medium:
			return AsteroidSize::Small;
		case AsteroidSize::Small:
			throw std::invalid_argument("Cannot split the smallest");
		default:
			throw std::invalid_argument("Unsupported asteroid size");
		}
	}

	static constexpr unsigned int AMOUNT_VERTICES = 8;
	static constexpr unsigned int SPLIT_ASTEROIDS_AMOUNT = 2;

	AsteroidSize m_size;
	Vec2f m_position;
	Vec2f m_velocity{ Randomizer::RandomRange(-80, 80), Randomizer::RandomRange(-80, 80) };
	float m_angle = 0;
	float m_angularVelocity = Randomizer::RandomRange(-80, 80);
	std::vector<Vec2f> m_vertices;
};