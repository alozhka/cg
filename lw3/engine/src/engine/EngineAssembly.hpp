#pragma once
#include "../shapes/Circle.hpp"
#include "../shapes/Rectangle.hpp"
#include "../shared/Color.hpp"
#include "../shared/CompositeObject.hpp"

#include <cmath>

// Константы для красивых цветов
namespace EngineColors
{
const Color Steel = { 0.60f, 0.60f, 0.65f, 1.0f };
const Color DarkSteel = { 0.40f, 0.40f, 0.45f, 1.0f };
const Color Aluminum = { 0.80f, 0.80f, 0.85f, 1.0f };
const Color Red = { 0.80f, 0.20f, 0.20f, 1.0f };
const Color DarkGrey = { 0.20f, 0.20f, 0.20f, 1.0f };
} // namespace EngineColors

class EngineAssembly : public CompositeObject
{
public:
	EngineAssembly()
	{
		BuildEngine();
	}

	void Update(double dt) override
	{
		// 1. Увеличиваем угол (имитация вращения)
		m_crankAngleDegrees += m_rpm * dt;
		if (m_crankAngleDegrees > 360.0f)
		{
			m_crankAngleDegrees -= 360.0f;
		}

		// Перевод в радианы для математики
		float alpha = m_crankAngleDegrees * 3.1415926f / 180.0f;

		// 2. Основная формула кривошипно-шатунного механизма
		// Позиция поршня по Y (от центра коленвала)
		// y = r * cos(a) + sqrt(l^2 - r^2 * sin^2(a))
		float sinA = std::sin(alpha);
		float cosA = std::cos(alpha);

		// Слагаемое, связанное с наклоном шатуна
		float rodVerticalComponent = std::sqrt(m_rodLength * m_rodLength - m_crankRadius * m_crankRadius * sinA * sinA);

		float pistonY = m_crankRadius * cosA + rodVerticalComponent;

		// 3. Вычисление угла наклона шатуна (Beta)
		// sin(beta) = (r / l) * sin(alpha)
		float sinBeta = (m_crankRadius / m_rodLength) * sinA;
		float betaRad = std::asin(sinBeta);
		float betaDeg = betaRad * 180.0f / 3.1415926f;

		// 4. Применение трансформаций

		// Коленвал: вращаем просто по углу (с минусом, чтобы крутился по часовой)
		if (m_crankShaftNode)
		{
			m_crankShaftNode->SetRotation(-m_crankAngleDegrees);
		}

		// Поршень: ходит только вверх-вниз
		if (m_pistonNode)
		{
			m_pistonNode->SetPosition(0.0f, pistonY);
		}

		// Шатун:
		// Он "висит" на поршне, поэтому его позиция (0,0) совпадает с центром пальца поршня.
		// Нам нужно только повернуть его на угол Beta.
		// Обрати внимание: наклон шатуна противоположен смещению коленвала по X,
		// но asin автоматически даёт правильный знак.
		if (m_rodNode)
		{
			m_rodNode->SetPosition(0.0f, pistonY);
			m_rodNode->SetRotation(betaDeg);
		}

		// Рекурсивный апдейт детей (если у них есть своя логика)
		CompositeObject::Update(dt);
	}

private:
	void BuildEngine()
	{
		// --- Параметры двигателя ---
		m_crankRadius = 35.0f;
		m_rodLength = 90.0f;

		// Размеры деталей
		float pistonW = 54.0f;
		float pistonH = 45.0f;
		float cylWallThickness = 10.0f;
		float cylHeight = 180.0f;

		// 1. Блок цилиндров (Статика)
		// Центр блока визуально сместим вверх, чтобы коленвал был внизу
		auto engineBlock = std::make_shared<CompositeObject>();

		// Левая стенка
		// Rect: x, y - центр фигуры
		engineBlock->AddChild(std::make_shared<Rectangle>(
			Rect{ -(pistonW / 2 + cylWallThickness / 2), m_crankRadius + 40, cylWallThickness, cylHeight },
			EngineColors::DarkGrey));

		// Правая стенка
		engineBlock->AddChild(std::make_shared<Rectangle>(
			Rect{ (pistonW / 2 + cylWallThickness / 2), m_crankRadius + 40, cylWallThickness, cylHeight },
			EngineColors::DarkGrey));

		AddChild(engineBlock);

		// 2. Коленчатый вал (Crankshaft)
		// Вращается вокруг (0,0) сцены
		m_crankShaftNode = std::make_shared<CompositeObject>();

		// Противовес (большой круг, смещенный вниз от центра вращения)
		auto counterWeight = std::make_shared<Circle>(
			Point{ 0, -20 },
			EngineColors::Steel,
			32.0f);
		m_crankShaftNode->AddChild(counterWeight);

		// Щека коленвала (прямоугольник, соединяющий центр и палец)
		m_crankShaftNode->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, m_crankRadius / 2, 15, m_crankRadius },
			EngineColors::Steel));

		// Шейка шатуна (Crank Pin) - красная точка крепления
		auto crankPin = std::make_shared<Circle>(
			Point{ 0, m_crankRadius },
			EngineColors::Red,
			8.0f);
		m_crankShaftNode->AddChild(crankPin);

		AddChild(m_crankShaftNode);

		// 3. Шатун (Connecting Rod)
		// Важный момент: Pivot point (0,0) этого узла - это верхняя точка (где поршень).
		// Поэтому геометрию внутри узла мы смещаем вниз.
		m_rodNode = std::make_shared<CompositeObject>();

		// Тело шатуна. Длина L. Центр прямоугольника должен быть на L/2 вниз.
		auto rodBody = std::make_shared<Rectangle>(
			Rect{ 0, -m_rodLength / 2.0f, 12.0f, m_rodLength },
			EngineColors::DarkSteel);
		m_rodNode->AddChild(rodBody);

		// Нижняя головка шатуна (крепится к коленвалу)
		auto rodBottomCap = std::make_shared<Circle>(
			Point{ 0, -m_rodLength },
			EngineColors::DarkSteel,
			14.0f);
		m_rodNode->AddChild(rodBottomCap);

		// Верхняя головка шатуна (крепится к поршню)
		auto rodTopCap = std::make_shared<Circle>(
			Point{ 0, 0 },
			EngineColors::DarkSteel,
			10.0f);
		m_rodNode->AddChild(rodTopCap);

		AddChild(m_rodNode);

		// 4. Поршень (Piston)
		m_pistonNode = std::make_shared<CompositeObject>();

		// Тело поршня
		m_pistonNode->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 0, pistonW, pistonH },
			EngineColors::Aluminum));

		// Палец поршня (визуальный центр крепления)
		m_pistonNode->AddChild(std::make_shared<Circle>(
			Point{ 0, 0 },
			EngineColors::DarkGrey,
			6.0f));

		// Кольца поршневые (для детализации)
		m_pistonNode->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 10, pistonW + 2, 3 }, EngineColors::DarkGrey));
		m_pistonNode->AddChild(std::make_shared<Rectangle>(
			Rect{ 0, 4, pistonW + 2, 3 }, EngineColors::DarkGrey));

		AddChild(m_pistonNode);
	}

private:
	// Ссылки на подвижные узлы для анимации
	std::shared_ptr<CompositeObject> m_crankShaftNode;
	std::shared_ptr<CompositeObject> m_pistonNode;
	std::shared_ptr<CompositeObject> m_rodNode;

	// Параметры
	float m_crankRadius;
	float m_rodLength;

	// Состояние
	float m_crankAngleDegrees = 0.0f;
	float m_rpm = 120.0f; // Оборотов в минуту (визуально, множитель скорости)
};