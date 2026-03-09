#pragma once
#include "../graphics/Color.hpp"
#include "../graphics/CompositeObject.hpp"
#include "../graphics/SceneObject.hpp"
#include "../shapes/Rectangle.hpp"
#include "ConnectingRod.hpp"
#include "CrankShaft.hpp"
#include "Pipe.hpp"
#include "Piston.hpp"
#include "SparkPlug.hpp"
#include "Valve.hpp"

#include <cmath>
#include <memory>
#include <numbers>

class Engine final : public CompositeObject
{
public:
	Engine()
	{
		BuildStaticBlock();
		BuildMovingParts();
		BuildCylinderHead();
	}

	void Update(float dt) override
	{
		CompositeObject::Update(dt);

		constexpr float R = 35.0f; // radius of CrankShaft
		constexpr float L = 95.0f; // length of ConnectingRod
		constexpr float crankShaftCenter = -50.0f;

		// 1. Update crank angle
		float speed = 180.0f; // degrees per second
		m_crankAngle += speed * dt;
		if (m_crankAngle > 360.0f)
		{
			m_crankAngle -= 360.0f;
		}

		m_crankShaft->SetRotation(m_crankAngle);

		// 2. Calculate Crank pin position
		float angleRad = m_crankAngle * std::numbers::pi_v<float> / 180.0f;
		float xc = -R * std::sin(angleRad);
		float yc = crankShaftCenter + R * std::cos(angleRad);

		// 3. Update ConnectingRod position (top head on crank pin)
		m_connRod->SetPosition(xc, yc);

		// 4. Calculate Piston position
		// yp = yc + sqrt(L^2 - xc^2)
		float yp = yc + std::sqrt(L * L - xc * xc);
		m_piston->SetPosition(0, yp + 5.0f);

		// 5. Calculate ConnectingRod rotation
		// phi = asin(xc / L)
		float phiRad = std::asin(xc / L);
		float phiDeg = phiRad * 180.0f / std::numbers::pi_v<float>;
		m_connRod->SetRotation(phiDeg);
	}

private:
	void BuildStaticBlock()
	{
		auto block = std::make_shared<CompositeObject>();

		auto carter = std::make_shared<Rectangle>(
			Vec2f{ 0, -50 },
			160,
			120,
			Palette::CastIron);
		block->AddChild(carter);

		auto cylinderBlock = std::make_shared<Rectangle>(
			Vec2f{ 0, 70 },
			100,
			140,
			Palette::CastIron);
		block->AddChild(cylinderBlock);

		auto cylinderLiner = std::make_shared<Rectangle>(
			Vec2f{ 0, 70 },
			62.0,
			140.0,
			Palette::CylinderInner);
		block->AddChild(cylinderLiner);

		AddChild(block);
	}

	void BuildMovingParts()
	{
		constexpr int crankShaftCenter = -50;

		m_crankShaft = std::make_unique<EngineParts::CrankShaft>(35.0);
		m_crankShaft->SetPosition(0, crankShaftCenter);
		AddChild(m_crankShaft);

		m_connRod = std::make_shared<EngineParts::ConnectingRod>(95.0);
		m_connRod->SetPosition(0, crankShaftCenter);
		AddChild(m_connRod);

		m_piston = std::make_shared<EngineParts::Piston>(60.0, 45.0);
		// Позиция: Центр коленвала + Радиус + Длина шатуна
		m_piston->SetPosition(0, crankShaftCenter + 35.0 + 95.0);
		AddChild(m_piston);
	}

	void BuildCylinderHead()
	{
		float headY = 155;

		auto headBlock = std::make_shared<Rectangle>(
			Vec2f{ 0, headY }, 120, 50,
			Palette::CastIron);
		AddChild(headBlock);

		auto intakePipe = std::make_shared<EngineParts::Pipe>(-20.0);
		intakePipe->SetPosition(-50, headY - 20);
		AddChild(intakePipe);

		auto exhaustPipe = std::make_shared<EngineParts::Pipe>(20);
		exhaustPipe->SetPosition(50, headY - 20);
		AddChild(exhaustPipe);

		float valveY = headY - 10;

		auto leftValve = std::make_shared<EngineParts::Valve>();
		leftValve->SetPosition(-15, valveY);
		AddChild(leftValve);

		auto rightValve = std::make_shared<EngineParts::Valve>();
		rightValve->SetPosition(15, valveY);
		AddChild(rightValve);

		auto sparkPlug = std::make_shared<EngineParts::SparkPlug>();
		sparkPlug->SetPosition(0, headY);
		AddChild(sparkPlug);
	}

	std::shared_ptr<EngineParts::CrankShaft> m_crankShaft;
	std::shared_ptr<EngineParts::Piston> m_piston;
	std::shared_ptr<EngineParts::ConnectingRod> m_connRod;
	float m_crankAngle = 0.0f;
};