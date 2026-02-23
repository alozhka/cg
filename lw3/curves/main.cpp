#include "src/app/GuiApplication.h"

#include <array>
#include <iostream>
#define GL_SILENCE_DEPRECATION

// Структура для хранения координат 2D точки
struct Point2D {
    float x, y;
};

// Глобальные контрольные точки (в нормализованных координатах устройства: от -1.0 до 1.0)
std::array<Point2D, 4> controlPoints = {
    Point2D{-0.8f, -0.5f}, // P0: Начало
    Point2D{-0.4f,  0.8f}, // P1: Первая контрольная точка
    Point2D{ 0.4f,  0.8f}, // P2: Вторая контрольная точка
    Point2D{ 0.8f, -0.5f}  // P3: Конец
};

// Вектор для хранения кэшированных точек самой кривой
std::vector<Point2D> curveVertices;

// Функция вычисления точек кубической кривой Безье
void calculateBezierCurve(int segments) {
    curveVertices.clear();

    for (int i = 0; i <= segments; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(segments);

        // Полиномы Бернштейна
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        // Формула кубической кривой Безье
        Point2D p;
        p.x = uuu * controlPoints[0].x + 3 * uu * t * controlPoints[1].x + 3 * u * tt * controlPoints[2].x + ttt * controlPoints[3].x;
        p.y = uuu * controlPoints[0].y + 3 * uu * t * controlPoints[1].y + 3 * u * tt * controlPoints[2].y + ttt * controlPoints[3].y;

        curveVertices.push_back(p);
    }
}

// Функция отрисовки сцены
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Рисуем пунктирные линии между контрольными точками
    glColor3f(0.5f, 0.5f, 0.5f); // Серый цвет
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);    // Паттерн пунктира
    glBegin(GL_LINE_STRIP);
    for (const auto& pt : controlPoints) {
        glVertex2f(pt.x, pt.y);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // 2. Рисуем саму аппроксимированную кривую Безье
    glColor3f(1.0f, 1.0f, 1.0f); // Белый цвет
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& pt : curveVertices) {
        glVertex2f(pt.x, pt.y);
    }
    glEnd();
    glLineWidth(1.0f); // Возвращаем толщину по умолчанию

    // 3. Рисуем контрольные точки
    glColor3f(1.0f, 0.0f, 0.0f); // Красный цвет
    glPointSize(8.0f);           // Делаем точки крупнее
    glBegin(GL_POINTS);
    for (const auto& pt : controlPoints) {
        glVertex2f(pt.x, pt.y);
    }
    glEnd();
}

int main()
{
	std::setlocale(LC_ALL, "ru_RU.UTF-8");
	GuiApplication app(800, 600, "Curves drawer");

	calculateBezierCurve(100);

	auto onDraw = [] {
		renderScene();
	};
	app.MainLoop(onDraw);

	return EXIT_SUCCESS;
}