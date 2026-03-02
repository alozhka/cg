#pragma once
#include "../shapes/Rectangle.hpp"
#include "../shared/Color.hpp"
#include "../shared/CompositeObject.hpp"
#include "../shared/SceneObject.hpp"
#include "ConnectingRod.hpp"
#include "CrankShaft.hpp"
#include "Pipe.hpp"
#include "Piston.hpp"
#include "SparkPlug.hpp"
#include "Valve.hpp"

#include <memory>
#include <vector>

class Engine final : public CompositeObject
{
public:
	Engine()
	{
		m_pistonWidth = 60.0;
		m_pistonHeight = 45.0;
		m_cylinderWidth = 62.0;
		m_cylinderHeight = 140.0;

		m_crankRadius = 35.0;
		m_rodLength = 95.0;

		BuildStaticBlock();
		BuildMovingParts();
		BuildCylinderHead();
	}

	void Update(double dt) override
	{
		CompositeObject::Update(dt);
	}

private:
	void BuildStaticBlock()
	{
		auto block = std::make_shared<CompositeObject>();

		auto carter = std::make_shared<Rectangle>(
			Point{ 0, -50 },
			160,
			120,
			Palette::CastIron);
		block->AddChild(carter);

		auto cylinderBlock = std::make_shared<Rectangle>(
			Point{ 0, 70 },
			100,
			140,
			Palette::CastIron);
		block->AddChild(cylinderBlock);

		auto cylinderLiner = std::make_shared<Rectangle>(
			Point{ 0, 70 },
			m_cylinderWidth,
			m_cylinderHeight,
			Palette::CylinderInner);
		block->AddChild(cylinderLiner);

		AddChild(block);
	}

	void BuildMovingParts()
	{
		constexpr int crankShaftCenter = -50;

		m_crankShaft = std::make_unique<EngineParts::CrankShaft>(m_crankRadius);
		m_crankShaft->SetPosition(0, crankShaftCenter);
		AddChild(m_crankShaft);

		m_connRod = std::make_shared<EngineParts::ConnectingRod>(m_rodLength);
		m_connRod->SetPosition(0, crankShaftCenter + m_crankRadius);
		AddChild(m_connRod);

		m_piston = std::make_shared<EngineParts::Piston>(m_pistonWidth, m_pistonHeight);
		// Позиция: Центр коленвала + Радиус + Длина шатуна
		m_piston->SetPosition(0, crankShaftCenter + m_crankRadius + m_rodLength);
		AddChild(m_piston);
	}

	void BuildCylinderHead()
	{
		auto headGroup = std::make_shared<CompositeObject>();
		double headY = 70 + m_cylinderHeight / 2.0 + 15;

		// 1. Головка блока (Крышка)
		headGroup->AddChild(std::make_shared<Rectangle>(
			Point{ 0, headY }, 120, 50,
			Palette::CastIron));

		AddChild(headGroup);

		// 2. Впускной канал (Труба слева) - повернутый прямоугольник
		auto intakePipe = std::make_shared<EngineParts::Pipe>(-20.0);
		intakePipe->SetPosition(-50, headY - 20);
		AddChild(intakePipe);

		auto exhaustPipe = std::make_shared<EngineParts::Pipe>(20);
		exhaustPipe->SetPosition(50, headY - 20);
		AddChild(exhaustPipe);

		// 4. Клапаны
		double valveY = headY - 10;

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

	std::shared_ptr<CompositeObject> m_crankShaft;
	std::shared_ptr<CompositeObject> m_piston;
	std::shared_ptr<CompositeObject> m_connRod;

	double m_pistonWidth;
	double m_pistonHeight;
	double m_cylinderWidth;
	double m_cylinderHeight;
	double m_crankRadius;
	double m_rodLength;
};