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

		UpdateCrankAngle(dt);
		m_crankShaft->SetRotation(m_crankAngle);

		Vec2f crankPinPos = ComputeCrankPinPosition(m_crankAngle);
		m_connRod->SetPosition(crankPinPos);

		Vec2f pistonPos = ComputePistonPosition(crankPinPos);
		m_piston->SetPosition(pistonPos);

		float degrees = ComputeConnectingRodRotation(crankPinPos);
		m_connRod->SetRotation(degrees);
	}

private:
	void UpdateCrankAngle(float dt)
	{
		m_crankAngle += ANGLE_SPEED * dt;
		if (m_crankAngle > 360.0f)
		{
			m_crankAngle -= 360.0f;
		}
	}

	static Vec2f ComputeCrankPinPosition(float angle)
	{
		float angleRad = angle * std::numbers::pi_v<float> / 180.0f;
		float x = -CRANKSHAFT_RADIUS * std::sin(angleRad);
		float y = CRANKSHAFT_CENTER + CRANKSHAFT_RADIUS * std::cos(angleRad);
		return { x, y };
	}

	static Vec2f ComputePistonPosition(Vec2f crankPinPosition)
	{
		// y = y0 + sqrt(L^2 - x0^2)
		float y = crankPinPosition.y + std::sqrt(std::pow(CONNECTING_ROD_LENGTH, 2) - std::pow(crankPinPosition.x, 2));
		return { 0, y };
	}

	static float ComputeConnectingRodRotation(Vec2f crankPinPosition)
	{
		// phi = asin(xc / L)
		float radians = std::asin(crankPinPosition.x / CONNECTING_ROD_LENGTH);
		float degrees = radians * 180.0f / std::numbers::pi_v<float>;
		return degrees;
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
		m_crankShaft = std::make_unique<EngineParts::CrankShaft>(CRANKSHAFT_RADIUS);
		m_crankShaft->SetPosition(0, CRANKSHAFT_CENTER);
		AddChild(m_crankShaft);

		m_connRod = std::make_shared<EngineParts::ConnectingRod>(CONNECTING_ROD_LENGTH);
		m_connRod->SetPosition(0, CRANKSHAFT_CENTER + 50);
		AddChild(m_connRod);

		m_piston = std::make_shared<EngineParts::Piston>(60.0, 45.0);
		// Позиция: Центр коленвала + Радиус + Длина шатуна
		m_piston->SetPosition(0, CRANKSHAFT_CENTER + CRANKSHAFT_RADIUS + CONNECTING_ROD_LENGTH);
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

	constexpr static float ANGLE_SPEED = 180;
	constexpr static float CRANKSHAFT_CENTER = -50;
	constexpr static float CRANKSHAFT_RADIUS = 35;
	constexpr static float CONNECTING_ROD_LENGTH = 100;

	float m_crankAngle = 0;
	std::shared_ptr<EngineParts::CrankShaft> m_crankShaft;
	std::shared_ptr<EngineParts::Piston> m_piston;
	std::shared_ptr<EngineParts::ConnectingRod> m_connRod;
};