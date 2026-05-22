#pragma once

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <limits>

// Освещённость, поступающая в данную точку от одного источника света.
struct LightSample
{
	// Направление от точки К источнику (единичный).
	glm::vec3 direction{ 0, 1, 0 };
	// Расстояние до источника. Для направленных — +inf.
	float distance = std::numeric_limits<float>::infinity();
	glm::vec3 ambient{ 0 };
	glm::vec3 diffuse{ 0 };
	glm::vec3 specular{ 0 };
};

class ILight
{
public:
	virtual ~ILight() = default;

	virtual LightSample Sample(const glm::vec3& point) const = 0;
};

class DirectLight : public ILight
{
public:
	DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: m_toLight(glm::normalize(-direction))
		, m_ambient(ambient)
		, m_diffuse(diffuse)
		, m_specular(specular)
	{
	}

	LightSample Sample(const glm::vec3&) const override
	{
		return LightSample{
			m_toLight,
			std::numeric_limits<float>::infinity(),
			m_ambient,
			m_diffuse,
			m_specular,
		};
	}

private:
	glm::vec3 m_toLight;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

class PointLight : public ILight
{
public:
	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: m_position(position)
		, m_ambient(ambient)
		, m_diffuse(diffuse)
		, m_specular(specular)
	{
	}

	LightSample Sample(const glm::vec3& point) const override
	{
		const glm::vec3 toLight = m_position - point;
		const float distance = glm::length(toLight);
		return LightSample{
			toLight / distance,
			distance,
			m_ambient,
			m_diffuse,
			m_specular,
		};
	}

private:
	glm::vec3 m_position;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};