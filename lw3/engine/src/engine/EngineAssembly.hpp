#pragma once
#include "../shapes/Circle.hpp"
#include "../shapes/Rectangle.hpp"
#include "../shared/Color.hpp"
#include "../shared/CompositeObject.hpp"
#include "../shared/Rect.h"
#include "../shared/SceneObject.hpp"
#include <cmath>
#include <memory>
#include <vector>

// Определим палитру, чтобы не хардкодить числа в логике
namespace Palette
{
const Color CastIron{ 0.70, 0.55, 0.45 }; // Светло-коричневый корпус (как на картинке)
const Color CastIronDark{ 0.55, 0.40, 0.30 }; // Темная окантовка
const Color CylinderInner{ 0.85, 0.90, 0.95 }; // Голубоватый металл внутри
const Color Steel{ 0.75, 0.75, 0.80 }; // Сталь (поршень)
const Color DarkSteel{ 0.50, 0.50, 0.55 }; // Темная сталь (шатун)
const Color CrankGreen{ 0.60, 0.70, 0.60 }; // Зеленоватый оттенок противовеса
const Color ValveColor{ 0.40, 0.30, 0.20 }; // Клапаны
const Color SparkPlug{ 0.90, 0.90, 0.80 }; // Свеча (керамика)
} // namespace Palette

class EngineAssembly : public CompositeObject
{
public:
	EngineAssembly()
	{
		// Настройка размеров (подгоняем под пропорции картинки)
		m_pistonWidth = 60.0;
		m_pistonHeight = 45.0;
		m_cylinderWidth = 62.0; // Чуть шире поршня
		m_cylinderHeight = 140.0;

		m_crankRadius = 35.0;
		m_rodLength = 95.0;

		BuildStaticBlock(); // Корпус
		BuildMovingParts(); // Механизм
		BuildCylinderHead(); // Голова (клапаны, впуск/выпуск)
	}

	// Пока без анимации, но структура готова
	void Update(double dt) override
	{
		CompositeObject::Update(dt);
	}

private:
	void BuildStaticBlock()
	{
		auto blockGroup = std::make_shared<CompositeObject>();

		// 1. Картер (Нижняя широкая часть)
		// Рисуем большой прямоугольник снизу
		blockGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, -50, 160, 120 },
			Palette::CastIron));

		// 2. Блок цилиндра (Средняя часть)
		blockGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 70, 100, 140 },
			Palette::CastIron));

		// 3. Гильза цилиндра (Внутренняя полость)
		// Это тот самый голубой прямоугольник, внутри которого ходит поршень
		blockGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 70, m_cylinderWidth, m_cylinderHeight },
			Palette::CylinderInner));

		AddChild(blockGroup);
	}

	void BuildMovingParts()
	{
		// --- Коленвал ---
		m_crankShaft = std::make_shared<CompositeObject>();
		m_crankShaft->SetPosition(0, -50); // Центр вращения внизу, в картере

		// Противовес (Зеленоватый сектор)
		// Имитируем форму "топора" двумя кругами и прямоугольником или просто большим кругом
		auto counterWeight = std::make_shared<Circle>(
			Point{ 0, -20 }, Palette::CrankGreen, 45.0);
		m_crankShaft->AddChild(counterWeight);

		// Ось вращения (маленький круг в центре)
		m_crankShaft->AddChild(std::make_shared<Circle>(
			Point{ 0, 0 }, Palette::DarkSteel, 10.0));

		// Шейка шатуна (Crank Pin) - точка крепления шатуна
		// Она смещена на радиус кривошипа
		m_crankShaft->AddChild(std::make_shared<Circle>(
			Point{ 0, m_crankRadius }, Palette::Steel, 8.0));

		AddChild(m_crankShaft);

		// --- Шатун ---
		m_connRod = std::make_shared<CompositeObject>();
		// По умолчанию ставим его в ВМТ (верхнюю точку)
		m_connRod->SetPosition(0, -50 + m_crankRadius);

		// Тело шатуна (Длинная палка)
		// Смещаем геометрию вниз, чтобы (0,0) узла было в точке крепления к поршню (как обсуждали)
		m_connRod->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, -m_rodLength / 2.0, 14, m_rodLength },
			Palette::DarkSteel));

		// Нижняя головка шатуна
		m_connRod->AddChild(std::make_shared<Circle>(
			Point{ 0, -m_rodLength }, Palette::DarkSteel, 16.0));

		// Верхняя головка шатуна
		m_connRod->AddChild(std::make_shared<Circle>(
			Point{ 0, 0 }, Palette::DarkSteel, 12.0));

		AddChild(m_connRod);

		// --- Поршень ---
		m_piston = std::make_shared<CompositeObject>();
		// Позиция: Центр коленвала + Радиус + Длина шатуна
		double startY = -50 + m_crankRadius + m_rodLength;
		m_piston->SetPosition(0, startY);

		// Основное тело
		m_piston->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 0, m_pistonWidth, m_pistonHeight },
			Palette::Steel));

		// Компрессионные кольца (Детализация - две темные полоски)
		m_piston->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 10, m_pistonWidth, 3 }, Palette::DarkSteel));
		m_piston->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 4, m_pistonWidth, 3 }, Palette::DarkSteel));

		// Палец поршня (В центре)
		m_piston->AddChild(std::make_shared<Circle>(
			Point{ 0, -5 }, Palette::DarkSteel, 8.0));

		AddChild(m_piston);
	}

	void BuildCylinderHead()
	{
		auto headGroup = std::make_shared<CompositeObject>();
		double headY = 70 + m_cylinderHeight / 2.0 + 15; // Чуть выше блока

		// 1. Головка блока (Крышка)
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, headY, 120, 50 },
			Palette::CastIron));

		// 2. Впускной канал (Труба слева) - повернутый прямоугольник
		auto intakePipe = std::make_shared<Rectangle>(
			Rect{ -50, headY + 10, 60, 15 }, Palette::CastIron, 20.0 // Поворот
		);
		headGroup->AddChild(intakePipe);

		// Внутренность трубы (воздух)
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ -50, headY + 10, 60, 8 }, Palette::CylinderInner, 20.0));

		// 3. Выпускной канал (Труба справа)
		auto exhaustPipe = std::make_shared<Rectangle>(
			Rect{ 50, headY + 10, 60, 15 }, Palette::CastIron, -20.0);
		headGroup->AddChild(exhaustPipe);

		// Внутренность трубы
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 50, headY + 10, 60, 8 }, Palette::CylinderInner, -20.0));

		// 4. Клапаны
		double valveY = headY - 10;
		// Левый клапан
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ -15, valveY, 4, 35 }, Palette::ValveColor));
		headGroup->AddChild(std::make_shared<Rectangle>( // Тарелка клапана
			Rect{ -15, valveY - 18, 16, 5 }, Palette::ValveColor));

		// Правый клапан
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 15, valveY, 4, 35 }, Palette::ValveColor));
		headGroup->AddChild(std::make_shared<Rectangle>( // Тарелка клапана
			Rect{ 15, valveY - 18, 16, 5 }, Palette::ValveColor));

		// 5. Свеча зажигания (По центру)
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, headY + 20, 10, 25 }, Palette::SparkPlug));
		// Электрод
		headGroup->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, headY, 3, 15 }, Palette::DarkSteel));

		AddChild(headGroup);
	}

	// Указатели для будущей анимации
	std::shared_ptr<CompositeObject> m_crankShaft;
	std::shared_ptr<CompositeObject> m_piston;
	std::shared_ptr<CompositeObject> m_connRod;

	// Параметры
	double m_pistonWidth;
	double m_pistonHeight;
	double m_cylinderWidth;
	double m_cylinderHeight;
	double m_crankRadius;
	double m_rodLength;
};