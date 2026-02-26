#pragma once
#include "../shared/CompositeObject.hpp"

namespace Engine
{
class CrankShaft final : public CompositeObject
{
public:
	explicit CrankShaft(double radius)
		: m_radius(radius)
	{
		Init();
	}

private:
	void Init()
	{
		m_root->SetPosition(0, -50); // Центр вращения внизу, в картере

		// Противовес
		auto counterWeight = std::make_shared<Circle>(
			Point{ 0, -20 }, Palette::CrankGreen, 45.0);
		m_root->AddChild(counterWeight);

		// Ось вращения
		m_root->AddChild(std::make_shared<Circle>(
			Point{ 0, 0 }, Palette::DarkSteel, 10.0));

		// Шейка шатуна
		m_root->AddChild(std::make_shared<Circle>(
			Point{ 0, m_radius }, Palette::Steel, 8.0));
	}

	double m_radius;
	std::unique_ptr<CompositeObject> m_root = std::make_unique<CompositeObject>();
};
}; // namespace Engine