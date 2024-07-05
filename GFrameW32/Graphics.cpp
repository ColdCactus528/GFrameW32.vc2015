#include "StdAfx.h"
#include "GF.h"

#ifndef M_PI
const double M_PI = 3.1415926535897932384626433832795;
#endif
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

// Ваша реализация необходимых вам функций
void DrawLine(int x0, int y0, int x1, int y1, RGBPIXEL color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int sx = (dx > 0) ? 1 : -1;
	int sy = (dy > 0) ? 1 : -1;
	dx = abs(dx);
	dy = abs(dy);
	int err = dx - dy;
	int e2;
	while (true)
	{
		gfSetPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

std::vector<iPoint> DrawPoligon(int n, RGBPIXEL color)
{
	// Задай случайным образом n точек на экране.
	// Занеси эти точки в вектор.
	// Построй многоугольник, соединяя их.
	// Соедини начальную точку и последнюю.
	// Вернуть вектор точек.

	// Обновляем srand
	srand(time(NULL));
	// Вектор точек
	std::vector<iPoint> points;
	// Заполняем вектор случайными точками
	for (int i = 0; i < n; i++)
	{
		iPoint point;
		point.x = rand() % gfGetWindowWidth();
		point.y = rand() % gfGetWindowHeight();
		points.push_back(point);
	}
	// Соединяем точки
	for (int i = 0; i < n - 1; i++)
	{
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
	}
	// Соединяем последнюю и первую точки
	DrawLine(points[n - 1].x, points[n - 1].y, points[0].x, points[0].y, color);
	return points;
}

std::vector<iPoint> DrawOwnPoligon(std::vector<iPoint> points, RGBPIXEL color)
{
	// Построй многоугольник, соединяя точки из вектора.
	// Соедини начальную точку и последнюю.
	// Вернуть вектор точек.

	// Соединяем точки
	for (int i = 0; i < points.size() - 1; i++)
	{
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
	}
	// Соединяем последнюю и первую точки
	DrawLine(points[points.size() - 1].x, points[points.size() - 1].y, points[0].x, points[0].y, color);
	return points;
}

bool IsConvexPoligon(std::vector<iPoint> points)
{
	// Вернуть true, если многоугольник выпуклый.
	// Вернуть false, если многоугольник не выпуклый.

	// Проверяем, выпуклый ли многоугольник
	// Переменная для хранения знака
	int sign = 0;
	// Переменная для хранения текущего знака
	int currentSign = 0;
	// Переменная для хранения количества точек
	int n = points.size();
	// Перебираем точки
	for (int i = 0; i < n; i++)
	{
		// Вычисляем знак
		currentSign = (points[(i + 1) % n].x - points[i].x) * (points[(i + 2) % n].y - points[i].y) - (points[(i + 1) % n].y - points[i].y) * (points[(i + 2) % n].x - points[i].x);
		// Если знак отличается от нуля
		if (currentSign != 0)
		{
			// Если знак отличается от предыдущего
			if (currentSign * sign < 0)
			{
				// Многоугольник не выпуклый
				return false;
			}
			// Запоминаем знак
			sign = currentSign;
		}
	}
	return true;
}

bool IsIntersect(iPoint a, iPoint b, iPoint c, iPoint d)
{
	// Вернуть true, если отрезки из этих точек пересекаются
	// Вернуть false, если отрезки из этих точек не пересекаются

	// Проверяем, пересекаются ли отрезки
	// Вычисляем знаки
	int sign1 = (c.x - a.x) * (b.y - a.y) - (b.x - a.x) * (c.y - a.y);
	int sign2 = (d.x - a.x) * (b.y - a.y) - (b.x - a.x) * (d.y - a.y);
	int sign3 = (a.x - c.x) * (d.y - c.y) - (d.x - c.x) * (a.y - c.y);
	int sign4 = (b.x - c.x) * (d.y - c.y) - (d.x - c.x) * (b.y - c.y);
	// Если знаки разные
	if (sign1 * sign2 < 0 && sign3 * sign4 < 0)
	{
		// Отрезки пересекаются
		return true;
	}
}

bool IsSimplePoligon(std::vector<iPoint> points)
{
	// Вернуть true, если многоугольник простой.
	// Вернуть false, если многоугольник не простой.

	// Проверяем, простой ли многоугольник
	// Переменная для хранения количества точек
	int n = points.size();
	// Перебираем точки
	for (int i = 0; i < n; i++)
	{
		// Перебираем точки
		for (int j = i + 2; j < n; j++)
		{
			// Если точки не совпадают
			if (i != j)
			{
				// Если отрезки пересекаются
				if (IsIntersect(points[i], points[(i + 1) % n], points[j], points[(j + 1) % n]))
				{
					// Многоугольник не простой
					return false;
				}
			}
		}
	}
	return true;
}

bool IsPointInPoligon(std::vector<iPoint> points, iPoint dot, bool flag) {
	//Если flag = true, то находится ли dot внутри многоугольника, заданного точками points определяется правилом EO
	//Если flag = false, то находится ли dot внутри многоугольника, заданного точками points определяется правилом  NZW

	if (flag == true) {
		// Переменная для хранения количества точек
		int n = points.size();
		// Переменная для хранения количества пересечений
		int count = 0;
		// Перебираем точки
		for (int i = 0; i < n; i++)
		{
			// Если отрезки пересекаются
			iPoint dot1 = points[i];
			iPoint dot2 = points[(i + 1) % n];
			if (dot1.y == dot2.y)
				continue;
			if (dot.y < min(dot1.y, dot2.y))
				continue;
			if (dot.y >= max(dot1.y, dot2.y))
				continue;
			double x = dot1.x + (dot.y - dot1.y) * (dot2.x - dot1.x) / (dot2.y - dot1.y);
			if (x > dot.x)
				count++;
		}
		// Если количество пересечений нечетное, то точка внутри многоугольника
		if (count % 2 == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		// Ввести новую переменную, которая хранит самую дальнюю координату по оси х 
		int maxX = std::max_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.x < b.x; })->x;

		// Перебрать все стороны многоульника
		// Узнаю пересеклась ли сторона многоугольника с лучом от точки dot до точки с самой дальней координатой по оси х
		// Если пересеклась, то узнаем направление обхода стороны многоугольника
		// Сравниваем координаты точек, которые лежат на стороне многоугольника и на луче
		// Если координата по y первой точки на стороне мноугольника больше чем координата по y второй точки на стороне многоугольника, то обход стороны многоугольника по часовой стрелке
		// Давляем в счетчик +1
		// Если нет, то -1
		// Если счетчик больше 0, то точка внутри многоугольника
		// Если счетчик меньше 0, то точка вне многоугольника

		// Переменная для хранения количества точек
		int n = points.size();
		// Переменная для хранения количества пересечений
		int count = 0;
		// Перебираем точки
		for (int i = 0; i < n; i++)
		{
			// Если отрезки пересекаются
			iPoint dot1 = points[i];
			iPoint dot2 = points[(i + 1) % n];
			if (dot1.y == dot2.y)
				continue;
			if (dot.y < min(dot1.y, dot2.y))
				continue;
			if (dot.y >= max(dot1.y, dot2.y))
				continue;
			double x = dot1.x + (dot.y - dot1.y) * (dot2.x - dot1.x) / (dot2.y - dot1.y);
			if (x > dot.x && dot1.y > dot2.y)
				count--;
			if (x > dot.x && dot1.y < dot2.y)
				count++;
		}
		if (count > 0) {
			return true;
		}
		else {
			return false;
		}
	}
}

void ColorPoligon(std::vector<iPoint> points, RGBPIXEL color, bool flag)
{
	// Определить максимальные и минимальные значения x и y в points, используя stl

	int minX = std::min_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.x < b.x; })->x;
	int maxX = std::max_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.x < b.x; })->x;
	int minY = std::min_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.y < b.y; })->y;
	int maxY = std::max_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.y < b.y; })->y;

	// Перебираем все точки внутри прямоугольника
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			// Если точка внутри многоугольника
			if (IsPointInPoligon(points, iPoint(x, y), flag))
			{
				// Закрашиваем точку
				gfSetPixel(x, y, color);
			}
		}
	}
}

//Функция факториала 
int Factorial(int n)
{
	if (n == 0)
		return 1;
	else
		return n * Factorial(n - 1);
}

// Функция полинома Бернстейна
double Bernstein(int n, int i, double t)
{
	return Factorial(n) / (Factorial(i) * Factorial(n - i)) * pow(t, i) * pow(1 - t, n - i);
}

int Dist(iPoint point) {
	return abs(point.x) + abs(point.y);
}

void BezierCurve3d(std::vector<iPoint> points, RGBPIXEL color) {
	// Нарисовать кубическую кривую Безье с помощью точек points
	// Цвет кривой color
	// Построить с помощью параметрического уравнения Безье
	// Параметр t принимает значения от 0 до 1
	// Параметр t увеличивается на 0.001

	// Переменная для хранения количества точек
	int n = points.size();
	// Переменная для хранения количества пересечений
	int count = 0;
	double xprev = points[0].x;
	double yprev = points[0].y;
	int h = max(Dist(points[0] - 2 * points[1] + points[2]), Dist(points[1] - 2 * points[2] + points[3]));
	int N = 1 + sqrt(3 * h);
	// Перебираем точки
	for (double t = 0; t < 1 + 1. / 2 / N; t += 1. / N)
	{
		double x = 0;
		double y = 0;
		for (int i = 0; i < n; i++)
		{
			x += points[i].x * Bernstein(n - 1, i, t);
			y += points[i].y * Bernstein(n - 1, i, t);
		}
		DrawLine(xprev, yprev, x, y, color);
		xprev = x;
		yprev = y;
	}
}

void BezierCurve(std::vector<iPoint> points, RGBPIXEL color) {
	// Нарисовать кубическую кривую Безье с помощью точек points
	// Цвет кривой color
	// Построить с помощью параметрического уравнения Безье
	// Параметр t принимает значения от 0 до 1
	// Параметр t увеличивается на 0.001

	// Переменная для хранения количества точек
	int n = points.size();
	// Переменная для хранения количества пересечений
	int count = 0;
	// Перебираем точки
	for (double t = 0; t <= 1; t += 0.001)
	{
		double x = 0;
		double y = 0;
		for (int i = 0; i < n; i++)
		{
			x += points[i].x * Bernstein(n - 1, i, t);
			y += points[i].y * Bernstein(n - 1, i, t);
		}
		gfSetPixel(x, y, color);
	}
}

iPoint Intersection(iPoint a1, iPoint a2, iPoint b1, iPoint b2)
{

	// Найти точку пересечения
	double x = ((a1.x * a2.y - a1.y * a2.x) * (b1.x - b2.x) - (a1.x - a2.x) * (b1.x * b2.y - b1.y * b2.x)) / ((a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x));
	double y = ((a1.x * a2.y - a1.y * a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x * b2.y - b1.y * b2.x)) / ((a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x));
	return iPoint(x, y);
}

iPoint FindNearestPoint(iPoint point, std::vector<iPoint> points) {
	// Найти ближайшую точку к точке point из массива points
	// Вернуть найденную точку
	// Если массив points пуст, вернуть точку (0, 0)
	if (points.size() == 0)
	{
		return iPoint(0, 0);
	}
	// Переменная для хранения минимального расстояния
	double min = 1000000;
	// Переменная для хранения индекса ближайшей точки
	int index = 0;
	// Перебираем точки
	for (int i = 0; i < points.size(); i++)
	{
		// Вычисляем расстояние
		double distance = sqrt(pow(points[i].x - point.x, 2) + pow(points[i].y - point.y, 2));
		// Если расстояние меньше минимального, то обновляем минимальное расстояние и индекс
		if (distance < min)
		{
			min = distance;
			index = i;
		}
	}
	// Возвращаем ближайшую точку
	return points[index];
}

// Функция возвращающая пересечение двух отрезков для алгоритма Сазерленда-Ходжмана
std::vector<iPoint> CuttingAlgorithmSHEdge(iPoint cutting_point1, iPoint cutting_point2, iPoint point1, iPoint point2)
{
	// Определение положения точек point1 и point2 относительно отрезка cutting_point1-cutting_point2
	iPoint::ClassifyResult classify_point1 = point1.Classify(cutting_point1, cutting_point2);
	iPoint::ClassifyResult classify_point2 = point2.Classify(cutting_point1, cutting_point2);

	// Если point1 и point2 находятся слева от прямой cutting_point1-cutting_point2, то возвращаем пустой вектор
	if (classify_point1 == iPoint::ClassifyResult::LEFT && classify_point2 == iPoint::ClassifyResult::LEFT)
		return std::vector<iPoint>();

	// Если point1 и point2 находятся справа от прямой cutting_point1-cutting_point2, то возвращаем point2
	if (classify_point1 == iPoint::ClassifyResult::RIGHT && classify_point2 == iPoint::ClassifyResult::RIGHT)
		return std::vector<iPoint>{ point2 };

	// Если point1 находится справа от прямой cutting_point1-cutting_point2, а point2 - слева, то возвращаем точку пересечения отрезка point1-point2 и прямой cutting_point1-cutting_point2 и point1
	if (classify_point1 == iPoint::ClassifyResult::RIGHT && classify_point2 == iPoint::ClassifyResult::LEFT)
	{
		double t = (point1.x - cutting_point1.x) * (cutting_point1.y - cutting_point2.y) - (point1.y - cutting_point1.y) * (cutting_point1.x - cutting_point2.x);
		t /= (point1.x - point2.x) * (cutting_point1.y - cutting_point2.y) - (point1.y - point2.y) * (cutting_point1.x - cutting_point2.x);
		return std::vector<iPoint>{ iPoint(point1.x + (point2.x - point1.x) * t, point1.y + (point2.y - point1.y) * t) };
	}

	// Если point1 находится на прямой cutting_point1-cutting_point2, а point2 - справа, то возвращаем point1 и point2
	if (classify_point1 != iPoint::ClassifyResult::LEFT && classify_point1 != iPoint::ClassifyResult::RIGHT && classify_point2 == iPoint::ClassifyResult::RIGHT)
		return std::vector<iPoint>{ point1, point2 };

	// Если point1 находится на прямой cutting_point1-cutting_point2, а point2 - слева, то возвращаем point1
	if (classify_point1 != iPoint::ClassifyResult::LEFT && classify_point1 != iPoint::ClassifyResult::RIGHT && classify_point2 == iPoint::ClassifyResult::LEFT)
		return std::vector<iPoint>{ point1 };

	// Если point2 находится на прямой cutting_point1-cutting_point2, а point1 - справа, то возвращаем point2
	if (classify_point2 != iPoint::ClassifyResult::LEFT && classify_point2 != iPoint::ClassifyResult::RIGHT && classify_point1 == iPoint::ClassifyResult::RIGHT)
		return std::vector<iPoint>{ point2 };

	// Если point2 находится на прямой cutting_point1-cutting_point2, а point1 - слева, то возвращаем пустой вектор
	if (classify_point2 != iPoint::ClassifyResult::LEFT && classify_point2 != iPoint::ClassifyResult::RIGHT && classify_point1 == iPoint::ClassifyResult::LEFT)
		return std::vector<iPoint>();

	// Если point1 и point2 находятся на прямой cutting_point1-cutting_point2, point2
	if (classify_point1 != iPoint::ClassifyResult::LEFT && classify_point1 != iPoint::ClassifyResult::RIGHT && classify_point2 != iPoint::ClassifyResult::LEFT && classify_point2 != iPoint::ClassifyResult::RIGHT)
		return std::vector<iPoint>{ point2 };

	// Если point1 находится слева от прямой cutting_point1-cutting_point2, а point2 - справа, то возвращаем точку пересечения отрезка point1-point2 и прямой cutting_point1-cutting_point2 и point2
	if (classify_point1 == iPoint::ClassifyResult::LEFT && classify_point2 == iPoint::ClassifyResult::RIGHT)
	{
		double t = (point1.x - cutting_point1.x) * (cutting_point1.y - cutting_point2.y) - (point1.y - cutting_point1.y) * (cutting_point1.x - cutting_point2.x);
		t /= (point1.x - point2.x) * (cutting_point1.y - cutting_point2.y) - (point1.y - point2.y) * (cutting_point1.x - cutting_point2.x);
		return std::vector<iPoint>{ iPoint(point1.x + (point2.x - point1.x) * t, point1.y + (point2.y - point1.y) * t), point2 };
	}
}

// Отсечение полигона другим полигоном алгоритмом Сазерленда-Ходжмана
std::vector<iPoint> CuttingAlgorithmSH(std::vector<iPoint>& points, std::vector<iPoint>& cutting_points)
{
	// Перебираем все ребра отсекателя
	for (int i = 0; i < cutting_points.size(); i++)
	{
		// Получаем точки, образующие ребро отсекателя
		iPoint cutting_point1 = cutting_points[i];
		iPoint cutting_point2 = cutting_points[(i + 1) % cutting_points.size()];

		// Создаем новый полигон, в который будем записывать точки, которые останутся после отсечения
		std::vector<iPoint> new_points;

		// Перебираем все ребра полигона
		for (int j = 0; j < points.size(); j++)
		{
			// Получаем точки, образующие ребро полигона
			iPoint point1 = points[j];
			iPoint point2 = points[(j + 1) % points.size()];

			// Получаем точки, которые останутся после отсечения ребра полигона ребром отсекателя
			std::vector<iPoint> new_points = CuttingAlgorithmSHEdge(cutting_point1, cutting_point2, point1, point2);

			// Добавляем точки в новый полигон
			for (int k = 0; k < new_points.size(); k++)
				new_points.push_back(new_points[k]);
		}

		// Заменяем полигон на новый полигон
		points = new_points;
	}

	return points;
}

// Отсечение отрезков прямых выпуклым полигоном
void CuttingAlgorithmSB(std::vector<iPoint>& cutting_points, iPoint& point1, iPoint& point2)
{
	DrawLine(point1.x, point1.y, point2.x, point2.y, RGBPIXEL(0, 255, 0));
	int n = cutting_points.size();
	std::vector<iPoint> vector_of_normals(n);
	// Заполняем вектор нормалей
	for (int i = 0; i < n; i++)
	{
		iPoint cutting_point1 = cutting_points[i];
		iPoint cutting_point2 = cutting_points[(i + 1) % n];
		vector_of_normals[i] = iPoint(cutting_point2.y - cutting_point1.y, cutting_point1.x - cutting_point2.x);
	}

	std::vector<double> enterings(n, 0);
	std::vector<double> leavings(n, 1);

	// Перебираем все ребра отсекателя
	for (int i = 0; i < n; i++)
	{
		// Получаем точки, образующие ребро отсекателя
		iPoint cutting_point1 = cutting_points[i];
		iPoint cutting_point2 = cutting_points[(i + 1) % n];

		// Получаем нормаль к ребру отсекателя
		iPoint normal = vector_of_normals[i];
		double d = normal.x * (point2.x - point1.x) + normal.y * (point2.y - point1.y);
		double t = -(normal.x * (point1.x - cutting_point1.x) + normal.y * (point1.y - cutting_point1.y));
		if (d == 0) {
			if (t < 0) return;
		}
		else {
			t /= d;
			if (d < 0) enterings[i] = max(enterings[i], t);
			else leavings[i] = min(leavings[i], t);
		}
	}

	double t = 1;
	for (int i = 0; i < n; i++)
		t = min(t, leavings[i]);

	if (t < enterings[0])
		return;

	iPoint point3 = iPoint(point1.x + t * (point2.x - point1.x), point1.y + t * (point2.y - point1.y));
	t = 0;
	for (int i = 0; i < n; i++)
		t = max(t, enterings[i]);

	if (t > leavings[0])
		return;

	iPoint point4 = iPoint(point1.x + t * (point2.x - point1.x), point1.y + t * (point2.y - point1.y));

	point1 = point3;
	point2 = point4;
	DrawLine(point3.x, point3.y, point4.x, point4.y, RGBPIXEL(0, 0, 255));
}

// Функция GetAngle возвращает угол между точками p1 p2 p3 в градусах
double GetAngle(iPoint& p1, iPoint& p2, iPoint& p3)
{
	double a = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
	double b = sqrt(pow(p2.x - p3.x, 2) + pow(p2.y - p3.y, 2));
	double c = sqrt(pow(p3.x - p1.x, 2) + pow(p3.y - p1.y, 2));
	return acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / 3.14159265358979323846;
}

void CuttingLineArbitraryPolygon(std::vector<iPoint> points, iPoint point1, iPoint point2, RGBPIXEL color)
{
	int dx = point2.x - point1.x;
	int dy = point2.y - point1.y;
	int sx = (dx > 0) ? 1 : -1;
	int sy = (dy > 0) ? 1 : -1;
	dx = abs(dx);
	dy = abs(dy);
	int err = dx - dy;
	int e2;
	while (true)
	{
		if (IsPointInPoligon(points, point1, true))
		{
			gfSetPixel(point1.x, point1.y, color);
		}

		if (point1.x == point2.x && point1.y == point2.y)
			break;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			point1.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			point1.y += sy;
		}
	}
}

// Функция GetIntersection возвращает true если отрезки пересекаются и false если нет
bool GetIntersection(iPoint& point1, iPoint& point2, iPoint& point3, iPoint& point4)
{
	double a1 = point2.y - point1.y;
	double b1 = point1.x - point2.x;
	double c1 = a1 * point1.x + b1 * point1.y;
	double a2 = point4.y - point3.y;
	double b2 = point3.x - point4.x;
	double c2 = a2 * point3.x + b2 * point3.y;
	double det = a1 * b2 - a2 * b1;
	if (det == 0)
		return false;
	else
	{
		double x = (b2 * c1 - b1 * c2) / det;
		double y = (a1 * c2 - a2 * c1) / det;
		if (x >= min(point1.x, point2.x) && x <= max(point1.x, point2.x) && y >= min(point1.y, point2.y) && y <= max(point1.y, point2.y) &&
			x >= min(point3.x, point4.x) && x <= max(point3.x, point4.x) && y >= min(point3.y, point4.y) && y <= max(point3.y, point4.y))
			return true;
		else
			return false;
	}
}

// Построение дуги окружности с помощью кривых Безье 3-го порядка
void DrawArc(iPoint& center, int radius, double start_angle, double end_angle, RGBPIXEL color)
{
	// Вычисляем углы
	double angle1 = start_angle * M_PI / 180;
	double angle2 = end_angle * M_PI / 180;

	// Вычисляем точки начала и конца дуги
	iPoint point1 = iPoint(center.x + radius * cos(angle1), center.y + radius * sin(angle1));
	iPoint point3 = iPoint(center.x + radius * cos(angle2), center.y + radius * sin(angle2));

	// Найдем радиус вектора
	iPoint radius_vector1 = iPoint(point1.x - center.x, point1.y - center.y);
	iPoint radius_vector2 = iPoint(point3.x - center.x, point3.y - center.y);

	// Найдем вектор нормали
	iPoint normal_vector1 = iPoint(-radius_vector1.y, radius_vector1.x);
	iPoint normal_vector2 = iPoint(-radius_vector2.y, radius_vector2.x);

	// Найдем точку пересечения касательных к окружности в точках начала и конца дуги
	iPoint point2 = point1 + normal_vector1;
	iPoint point4 = point3 + normal_vector2;

	// Найдем точку пересечения прямых point1 point3 и point2 point4
	iPoint point_tangent;
	point_tangent.x = ((point1.x * point2.y - point1.y * point2.x) * (point3.x - point4.x) - (point1.x - point2.x) * (point3.x * point4.y - point3.y * point4.x)) / ((point1.x - point2.x) * (point3.y - point4.y) - (point1.y - point2.y) * (point3.x - point4.x));
	point_tangent.y = ((point1.x * point2.y - point1.y * point2.x) * (point3.y - point4.y) - (point1.y - point2.y) * (point3.x * point4.y - point3.y * point4.x)) / ((point1.x - point2.x) * (point3.y - point4.y) - (point1.y - point2.y) * (point3.x - point4.x));

	// Вычислим расстояние от point_tangent до point1
	double distance = sqrt((point_tangent.x - point1.x) * (point_tangent.x - point1.x) + (point_tangent.y - point1.y) * (point_tangent.y - point1.y));

	// Вычислим точку P1
	iPoint point_p1;
	point_p1.x = point1.x + (point_tangent.x - point1.x) * 4 / 3 * (1 / (1 + sqrt(1 + (distance / radius) * (distance / radius))));
	point_p1.y = point1.y + (point_tangent.y - point1.y) * 4 / 3 * (1 / (1 + sqrt(1 + (distance / radius) * (distance / radius))));

	// Вычислим точку P2
	iPoint point_p2;
	point_p2.x = point3.x + (point_tangent.x - point3.x) * 4 / 3 * (1 / (1 + sqrt(1 + (distance / radius) * (distance / radius))));
	point_p2.y = point3.y + (point_tangent.y - point3.y) * 4 / 3 * (1 / (1 + sqrt(1 + (distance / radius) * (distance / radius))));

	std::vector<iPoint> points;
	points.push_back(point1);
	points.push_back(point_p1);
	points.push_back(point_p2);
	points.push_back(point3);

	BezierCurve3d(points, color);
	DrawLine(center.x, center.y, point1.x, point1.y, color);
	DrawLine(center.x, center.y, point3.x, point3.y, color);
	DrawLine(center.x, center.y, point_tangent.x, point_tangent.y, color);
}

// Построение параллельной проекции повернутого параллелепипеда на плоскость Z=n
void BuildParallelProjection(
	double n,
	double x0, double y0, double z0,
	double angle_x, double angle_y, double angle_z,
	dVector dir, double angle,
	RGBPIXEL color,
	double width, double height, double depth,
	bool isWireframe
)
{
	// Создание массива точек параллелепипеда
	std::vector<dVector4> points = {
		{ -0.5, -0.5, -0.5, 1},
		{ 0.5, -0.5, -0.5, 1},
		{ 0.5, 0.5, -0.5, 1},
		{ -0.5, 0.5, -0.5, 1},
		{ -0.5, -0.5, 0.5, 1},
		{ 0.5, -0.5, 0.5, 1},
		{ 0.5, 0.5, 0.5, 1},
		{ -0.5, 0.5, 0.5, 1}
	};

	// Создание массива внутренних нормалей граней
	std::vector<dVector4> normals = {
		{ 0, 0, 1, 1 },
		{ 0, 0, -1, 1 },
		{ 0, 1, 0, 1 },
		{ -1, 0, 0, 1 },
		{ 0, -1, 0, 1 },
		{ 1, 0, 0, 1 }
	};

	// Создание матрицы масштабирования
	dMatrix scale = dMatrix::ScalingTransform({ width, height, depth });

	// Масштабирование точек
	for (auto& point : points)
	{
		point = scale.TransformAffineHomomorphic(point);
	}

	// Создание матрицы поворота на угол alpha_x вокруг оси X
	dMatrix rotationX = dMatrix::RotationXTransform(angle_x);

	// Создание матрицы поворота на угол alpha_y вокруг оси Y
	dMatrix rotationY = dMatrix::RotationYTransform(angle_y);

	// Создание матрицы поворота на угол alpha_z вокруг оси Z
	dMatrix rotationZ = dMatrix::RotationZTransform(angle_z);

	// Создание матрицы поворота вокруг произвольной оси
	dir = dir.Normalize();
	dMatrix rotationDir = dMatrix::RotationTransform(dir, angle);

	// Создание матрицы поворота
	dMatrix rotation = rotationX * rotationY * rotationZ * rotationDir;

	// Поворот всех точек параллелепипеда
	for (auto& point : points)
	{
		point = rotation.TransformAffineHomomorphic(point);
	}
	for (auto& normal : normals)
	{
		normal = rotation.TransformAffineHomomorphic(normal);
	}

	// Создание матрицы проекции
	dMatrix projection = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, n, 1
	};

	// Проекция всех точек параллелепипеда
	for (auto& point : points)
	{
		point = projection.TransformAffineHomomorphic(point);
	}

	// Создание матрицы переноса
	dMatrix translation = dMatrix::MovementTransform({ x0, y0, z0 });

	// Перенос всех точек параллелепипеда
	for (auto& point : points)
	{
		point = translation.TransformAffineHomomorphic(point);
	}

	if (isWireframe)
	{
		// Вывод параллелепипеда на экран
		for (int i = 0; i < 4; i++)
		{
			// Содание iPoints - массива точек для вывода на экран
			std::vector<iPoint> iPoints = {
				{ int(points[i].x), int(points[i].y) },
				{ int(points[(i + 1) % 4].x), int(points[(i + 1) % 4].y) },
				{ int(points[(i + 1) % 4 + 4].x), int(points[(i + 1) % 4 + 4].y) },
				{ int(points[i + 4].x), int(points[i + 4].y) }
			};
			// Построение линий
			for (int j = 0; j < 4; j++)
			{
				DrawLine(iPoints[j].x, iPoints[j].y, iPoints[(j + 1) % 4].x, iPoints[(j + 1) % 4].y, color);
			}
		}
	}
	else
	{
		// Создание массива граней
		std::vector<std::vector<int>> faces = {
			{ 0, 1, 2, 3 },
			{ 4, 5, 6, 7 },
			{ 0, 1, 5, 4 },
			{ 1, 2, 6, 5 },
			{ 2, 3, 7, 6 },
			{ 3, 0, 4, 7 }
		};

		// Создание вектора наблюдателя
		dVector observer = { 0, 0, 1 };

		// Вывод граней на экран при условии, что скалярное произведение нормали грани и вектора наблюдателя больше нуля
		for (int i = 0; i < 6; i++)
		{
			if (normals[i].z < 0)
			{
				// Создание массива точек для вывода на экран
				std::vector<iPoint> iPoints = {
					{ int(points[faces[i][0]].x), int(points[faces[i][0]].y) },
					{ int(points[faces[i][1]].x), int(points[faces[i][1]].y) },
					{ int(points[faces[i][2]].x), int(points[faces[i][2]].y) },
					{ int(points[faces[i][3]].x), int(points[faces[i][3]].y) }
				};
				// Построение линий грани
				for (int j = 0; j < 4; j++)
				{
					DrawLine(iPoints[j].x, iPoints[j].y, iPoints[(j + 1) % 4].x, iPoints[(j + 1) % 4].y, color);
				}
			}
		}
	}
}

// Построение одноточечной перспективной проекции повернутого параллелепипеда с точкой схода (0, 0, k)
void BuildPerspectiveProjection(
	double k,
	double x0, double y0, double z0,
	double angle_x, double angle_y, double angle_z,
	dVector dir, double angle,
	RGBPIXEL color,
	double width, double height, double depth,
	bool isWireframe
)
{
	// Создание массива точек параллелепипеда
	std::vector<dVector4> points = {
		{ -0.5, -0.5, -0.5, 1},
		{ 0.5, -0.5, -0.5, 1},
		{ 0.5, 0.5, -0.5, 1},
		{ -0.5, 0.5, -0.5, 1},
		{ -0.5, -0.5, 0.5, 1},
		{ 0.5, -0.5, 0.5, 1},
		{ 0.5, 0.5, 0.5, 1},
		{ -0.5, 0.5, 0.5, 1}
	};

	// Создание точки схода
	dVector4 pointOfView = { 0, 0, k, 1 };

	// Создание матрицы масштабирования
	dMatrix scale = dMatrix::ScalingTransform({ width, height, depth });

	// Масштабирование точек
	for (auto& point : points)
	{
		point = scale.TransformAffineHomomorphic(point);
	}
	pointOfView = scale.TransformAffineHomomorphic(pointOfView);

	// Создание матрицы поворота на угол alpha_x вокруг оси X
	dMatrix rotationX = dMatrix::RotationXTransform(angle_x);

	// Создание матрицы поворота на угол alpha_y вокруг оси Y
	dMatrix rotationY = dMatrix::RotationYTransform(angle_y);

	// Создание матрицы поворота на угол alpha_z вокруг оси Z
	dMatrix rotationZ = dMatrix::RotationZTransform(angle_z);

	// Создание матрицы поворота вокруг произвольной оси
	dir = dir.Normalize();
	dMatrix rotationDir = dMatrix::RotationTransform(dir, angle);

	// Создание матрицы поворота
	dMatrix rotation = rotationX * rotationY * rotationZ * rotationDir;

	// Поворот всех точек параллелепипеда
	for (auto& point : points)
	{
		point = rotation.TransformAffineHomomorphic(point);
	}
	pointOfView = rotation.TransformAffineHomomorphic(pointOfView);

	// Создание матрицы проекции
	dMatrix projection = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 1 / k,
		0, 0, 0, 1
	};

	// Проекция всех точек параллелепипеда
	for (auto& point : points)
	{
		point = projection.TransformAffineHomomorphic(point);
	}
	pointOfView = projection.TransformAffineHomomorphic(pointOfView);

	// Создание матрицы переноса
	dMatrix translation = dMatrix::MovementTransform({ x0, y0, z0 });

	// Перенос всех точек параллелепипеда
	for (auto& point : points)
	{
		point = translation.TransformAffineHomomorphic(point);
	}
	pointOfView = translation.TransformAffineHomomorphic(pointOfView);

	if (isWireframe)
	{
		// Вывод параллелепипеда на экран
		for (int i = 0; i < 4; i++)
		{
			// Содание iPoints - массива точек для вывода на экран
			std::vector<iPoint> iPoints = {
				{ int(points[i].x), int(points[i].y) },
				{ int(points[(i + 1) % 4].x), int(points[(i + 1) % 4].y) },
				{ int(points[(i + 1) % 4 + 4].x), int(points[(i + 1) % 4 + 4].y) },
				{ int(points[i + 4].x), int(points[i + 4].y) }
			};
			// Построение линий
			for (int j = 0; j < 4; j++)
			{
				DrawLine(iPoints[j].x, iPoints[j].y, iPoints[(j + 1) % 4].x, iPoints[(j + 1) % 4].y, color);
			}
		}

		// Построить линии из точки схода в точки параллелепипеда
		for (int i = 0; i < 4; i++)
		{
			DrawLine(int(pointOfView.x), int(pointOfView.y), int(points[i].x), int(points[i].y), RGBPIXEL::Red());
		}
	}
	else
	{
		// Создание массива граней
		std::vector<std::vector<int>> faces = {
			{ 0, 1, 2, 3 },
			{ 4, 5, 6, 7 },
			{ 0, 1, 5, 4 },
			{ 1, 2, 6, 5 },
			{ 2, 3, 7, 6 },
			{ 3, 0, 4, 7 }
		};

		// Создание функции определения вектора по двум точкам
		auto VectorByTwoPoints = [](dVector4 a, dVector4 b) -> dVector
		{
			return {
				b.x - a.x,
				b.y - a.y,
				b.z - a.z
			};
		};

		// Создание массива нормалей как векторное произведение ребер граней
		std::vector<dVector> faceNormals = {
			VectorByTwoPoints(points[0], points[1]) ^ VectorByTwoPoints(points[0], points[3]),
			VectorByTwoPoints(points[4], points[7]) ^ VectorByTwoPoints(points[4], points[5]),
			VectorByTwoPoints(points[0], points[4]) ^ VectorByTwoPoints(points[0], points[1]),
			VectorByTwoPoints(points[1], points[5]) ^ VectorByTwoPoints(points[1], points[2]),
			VectorByTwoPoints(points[2], points[6]) ^ VectorByTwoPoints(points[2], points[3]),
			VectorByTwoPoints(points[3], points[7]) ^ VectorByTwoPoints(points[3], points[0])
		};

		// Вывод граней на экран при условии, что скалярное произведение нормали грани и вектора наблюдателя больше нуля
		for (int i = 0; i < 6; i++)
		{
			if (faceNormals[i].z < 0)
			{
				// Создание массива точек для вывода на экран
				std::vector<iPoint> iPoints = {
					{ int(points[faces[i][0]].x), int(points[faces[i][0]].y) },
					{ int(points[faces[i][1]].x), int(points[faces[i][1]].y) },
					{ int(points[faces[i][2]].x), int(points[faces[i][2]].y) },
					{ int(points[faces[i][3]].x), int(points[faces[i][3]].y) }
				};
				// Построение линий грани
				for (int j = 0; j < 4; j++)
				{
					DrawLine(iPoints[j].x, iPoints[j].y, iPoints[(j + 1) % 4].x, iPoints[(j + 1) % 4].y, color);
				}
			}
		}
	}
}

// Вызывается один раз в самом начале при инициализации приложения
bool gfInitScene()
{
	gfSetWindowSize(600, 600);

	// Начертание дуги окружности
	//DrawArc(iPoint(300, 300), 150, 30, 120, RGBPIXEL::Red());

	// ДЗ с отсечением отрезка прямой произвольным полигоном
	// Нарисовать полигон в виде звезды
	/*std::vector<iPoint> points = { iPoint(100, 100), iPoint(200, 300), iPoint(600, 200),
								   iPoint(400, 100), iPoint(250, 100), iPoint(400, 200),
								   iPoint(550, 300), iPoint(100, 300), iPoint(300, 400),
								   iPoint(200, 300), iPoint(100, 300), iPoint(200, 200) };
	DrawOwnPoligon(points, RGBPIXEL(255, 0, 0));
	iPoint p1 = iPoint(50, 100);
	iPoint p2 = iPoint(600, 380);

	DrawLine(p1.x, p1.y, p2.x, p2.y, RGBPIXEL(0, 255, 0));
	CuttingLineArbitraryPolygon(points, p1, p2, RGBPIXEL(0, 0, 255));
	*/

	return true;
}


// Вызывается в цикле до момента выхода из приложения.
// Следует использовать для создания анимационных эффектов
void gfDrawScene()
{
	//3 лабораторная работа
   // Очистка экрана
	gfClearScreen(RGBPIXEL(255, 182, 0));

	static double angle = 0;
	BuildParallelProjection(
		0, // n
		gfGetWindowWidth() / 4, gfGetWindowHeight() / 6, 0, // x0, y0, z0 
		M_PI / 10, -M_PI / 8, M_PI / 2, // angle_x, angle_y, angle_z
		{ 1, 0, 0 }, 0, // dir, angle
		RGBPIXEL(45, 0, 247), // color
		75, 100, 100, // width, height, depth
		true // isWireframe
	);

	BuildPerspectiveProjection(
		-200, // k
		3 * gfGetWindowWidth() / 4, gfGetWindowHeight() / 6, 0, // x0, y0, z0 
		M_PI / 10, -M_PI / 8, M_PI / 2, // angle_x, angle_y, angle_z
		{ 1, 0, 0 }, 0, // dir, angle
		RGBPIXEL(106, 0, 244), // color 
		75, 100, 100, // width, height, depth
		true // isWireframe
	);
	BuildParallelProjection(
		0, // n
		gfGetWindowWidth() / 4, 3 * gfGetWindowHeight() / 6, 0, // x0, y0, z0
		M_PI / 10, -M_PI / 8, M_PI / 2, // angle_x, angle_y, angle_z
		{ 1, 0, 0 }, 0, // dir, angle
		RGBPIXEL(229, 0, 164), // color
		75, 100, 100, // width, height, depth
		false // isWireframe
	);
	BuildPerspectiveProjection(
		-200, // k
		3 * gfGetWindowWidth() / 4, 3 * gfGetWindowHeight() / 6, 0, // x0, y0, z0
		M_PI / 10, -M_PI / 8, M_PI / 2, // angle_x, angle_y, angle_z
		{ 1, 0, 0 }, 0, // dir, angle
		RGBPIXEL(137, 0, 242), // color 
		75, 100, 100, // width, height, depth
		false // isWireframe
	);
	BuildParallelProjection(
		0, // n
		gfGetWindowWidth() / 4, 5 * gfGetWindowHeight() / 6, 0, // x0, y0, z0
		M_PI / 10, -M_PI / 8, M_PI / 2, // angle_x, angle_y, angle_z
		{ 0, 1, 0 }, angle, // dir, angle
		RGBPIXEL(242, 0, 137), // color
		75, 100, 100, // width, height, depth
		false // isWireframe
	);
	BuildPerspectiveProjection(
		-200, // k
		3 * gfGetWindowWidth() / 4, 5 * gfGetWindowHeight() / 6, 0, // x0, y0, z0
		M_PI / 10, -M_PI / 8, M_PI / 2, // angle_x, angle_y, angle_z
		{ 0, 1, 0 }, angle, // dir, angle
		RGBPIXEL(242, 0, 137), // color
		75, 100, 100, // width, height, depth
		false // isWireframe
	);
	angle += 0.005;
	if (angle > 2 * M_PI)
	{
		angle -= 2 * M_PI;
	}

}

// Вызывается один раз перед выходом из приложения.
// Следует использовать для освобождения выделенных
// ресурсов (памяти, файлов и т.п.)
void gfCleanupScene()
{
}

// Вызывается когда пользователь нажимает левую кнопку мыши
void gfOnLMouseClick(int x, int y)
{
	x; y;
	gfDrawRectangle(x - 10, y - 10, x + 10, y + 10, RGBPIXEL::Green());
}

// Вызывается когда пользователь нажимает правую кнопку мыши
void gfOnRMouseClick(int x, int y)
{
	x; y;
}

// Вызывается когда пользователь нажимает клавишу на клавиатуре
void gfOnKeyDown(UINT key)
{
	key;

	if (key == 'A')
		gfDisplayMessage("'A' key has been pressed");
}

// Вызывается когда пользователь отжимает клавишу на клавиатуре
void gfOnKeyUp(UINT key)
{
	key;

	//if( key == 'B' )
	//    gfDisplayMessage( "'B' key has been un-pressed" );
}
