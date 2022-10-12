#include "StdAfx.h"
#include "GF.h"

#ifndef M_PI
const double M_PI = 3.1415926535897932384626433832795;
#endif
#include <vector>
#include <algorithm>
#include <iostream>

// ���� ���������� ����������� ��� �������
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
	// ����� ��������� ������� n ����� �� ������.
	// ������ ��� ����� � ������.
	// ������� �������������, �������� ��.
	// ������� ��������� ����� � ���������.
	// ������� ������ �����.

	// ��������� srand
	srand(time(NULL));
	// ������ �����
	std::vector<iPoint> points;
	// ��������� ������ ���������� �������
	for (int i = 0; i < n; i++)
	{
		iPoint point;
		point.x = rand() % gfGetWindowWidth();
		point.y = rand() % gfGetWindowHeight();
		points.push_back(point);
	}
	// ��������� �����
	for (int i = 0; i < n - 1; i++)
	{
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
	}
	// ��������� ��������� � ������ �����
	DrawLine(points[n - 1].x, points[n - 1].y, points[0].x, points[0].y, color);
	return points;
}

std::vector<iPoint> DrawOwnPoligon(std::vector<iPoint> points, RGBPIXEL color)
{
	// ������� �������������, �������� ����� �� �������.
	// ������� ��������� ����� � ���������.
	// ������� ������ �����.

	// ��������� �����
	for (int i = 0; i < points.size() - 1; i++)
	{
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
	}
	// ��������� ��������� � ������ �����
	DrawLine(points[points.size() - 1].x, points[points.size() - 1].y, points[0].x, points[0].y, color);
	return points;
}

bool IsConvexPoligon(std::vector<iPoint> points)
{
	// ������� true, ���� ������������� ��������.
	// ������� false, ���� ������������� �� ��������.

	// ���������, �������� �� �������������
	// ���������� ��� �������� �����
	int sign = 0;
	// ���������� ��� �������� �������� �����
	int currentSign = 0;
	// ���������� ��� �������� ���������� �����
	int n = points.size();
	// ���������� �����
	for (int i = 0; i < n; i++)
	{
		// ��������� ����
		currentSign = (points[(i + 1) % n].x - points[i].x) * (points[(i + 2) % n].y - points[i].y) - (points[(i + 1) % n].y - points[i].y) * (points[(i + 2) % n].x - points[i].x);
		// ���� ���� ���������� �� ����
		if (currentSign != 0)
		{
			// ���� ���� ���������� �� �����������
			if (currentSign * sign < 0)
			{
				// ������������� �� ��������
				return false;
			}
			// ���������� ����
			sign = currentSign;
		}
	}
	return true;
}
bool IsIntersect(iPoint a, iPoint b, iPoint c, iPoint d)
{
	// ������� true, ���� ������� �� ���� ����� ������������
	// ������� false, ���� ������� �� ���� ����� �� ������������

	// ���������, ������������ �� �������
	// ��������� �����
	int sign1 = (c.x - a.x) * (b.y - a.y) - (b.x - a.x) * (c.y - a.y);
	int sign2 = (d.x - a.x) * (b.y - a.y) - (b.x - a.x) * (d.y - a.y);
	int sign3 = (a.x - c.x) * (d.y - c.y) - (d.x - c.x) * (a.y - c.y);
	int sign4 = (b.x - c.x) * (d.y - c.y) - (d.x - c.x) * (b.y - c.y);
	// ���� ����� ������
	if (sign1 * sign2 < 0 && sign3 * sign4 < 0)
	{
		// ������� ������������
		return true;
	}
}

bool IsSimplePoligon(std::vector<iPoint> points)
{
	// ������� true, ���� ������������� �������.
	// ������� false, ���� ������������� �� �������.

	// ���������, ������� �� �������������
	// ���������� ��� �������� ���������� �����
	int n = points.size();
	// ���������� �����
	for (int i = 0; i < n; i++)
	{
		// ���������� �����
		for (int j = i + 2; j < n; j++)
		{
			// ���� ����� �� ���������
			if (i != j)
			{
				// ���� ������� ������������
				if (IsIntersect(points[i], points[(i + 1) % n], points[j], points[(j + 1) % n]))
				{
					// ������������� �� �������
					return false;
				}
			}
		}
	}
	return true;
}

bool IsPointInPoligon(std::vector<iPoint> points, iPoint dot, bool flag) {
	//���� flag = true, �� ��������� �� dot ������ ��������������, ��������� ������� points ������������ �������� EO
	//���� flag = false, �� ��������� �� dot ������ ��������������, ��������� ������� points ������������ ��������  NZW
	
	if (flag == true) {
		// ���������� ��� �������� ���������� �����
		int n = points.size();
		// ���������� ��� �������� ���������� �����������
		int count = 0;
		// ���������� �����
		for (int i = 0; i < n; i++)
		{
			// ���� ������� ������������
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
		// ���� ���������� ����������� ��������, �� ����� ������ ��������������
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
		// ������ ����� ����������, ������� ������ ����� ������� ���������� �� ��� � 
		int maxX = std::max_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.x < b.x; })->x;

		// ��������� ��� ������� ������������
		// ����� ����������� �� ������� �������������� � ����� �� ����� dot �� ����� � ����� ������� ����������� �� ��� �
		// ���� �����������, �� ������ ����������� ������ ������� ��������������
		// ���������� ���������� �����, ������� ����� �� ������� �������������� � �� ����
		// ���� ���������� �� y ������ ����� �� ������� ������������ ������ ��� ���������� �� y ������ ����� �� ������� ��������������, �� ����� ������� �������������� �� ������� �������
		// ������� � ������� +1
		// ���� ���, �� -1
		// ���� ������� ������ 0, �� ����� ������ ��������������
		// ���� ������� ������ 0, �� ����� ��� ��������������
		
		// ���������� ��� �������� ���������� �����
		int n = points.size();
		// ���������� ��� �������� ���������� �����������
		int count = 0;
		// ���������� �����
		for (int i = 0; i < n; i++)
		{
			// ���� ������� ������������
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
	// ���������� ������������ � ����������� �������� x � y � points, ��������� stl

	int minX = std::min_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.x < b.x; })->x;
	int maxX = std::max_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.x < b.x; })->x;
	int minY = std::min_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.y < b.y; })->y;
	int maxY = std::max_element(points.begin(), points.end(), [](iPoint a, iPoint b) { return a.y < b.y; })->y;

	// ���������� ��� ����� ������ ��������������
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			// ���� ����� ������ ��������������
			if (IsPointInPoligon(points, iPoint(x, y), flag))
			{
				// ����������� �����
				gfSetPixel(x, y, color);
			}
		}
	}
}

// ���������� ���� ��� � ����� ������ ��� ������������� ����������
bool gfInitScene()
{
	gfSetWindowSize(640, 480);
	
	// ������� ������� ������� �� 7 � ������� 0,0 - 320, 480
	// ������� ��� 1 �����, �� � ������� 320, 0 - 640, 480
	
	std::vector<iPoint> firstPoligon = {
		iPoint(10, 20),
		iPoint(20, 10),
		iPoint(280, 320),
		iPoint(15, 300),
		iPoint(280, 120),
		iPoint(300, 200),
		iPoint(40, 400),	
	};

	std::vector<iPoint> secondPoligon = {
	iPoint(320 + 10, 20),
	iPoint(320 + 20, 10),
	iPoint(320 + 280, 320),
	iPoint(320 + 15, 300),
	iPoint(320 + 280, 120),
	iPoint(320 + 300, 200),
	iPoint(320 + 40, 400),
	};

	// ��������� ��������� �� ����� ������ �������� firstPoligon
	// ���� ����� ������ ��������, �� ��������� �� � ������� ����
	// ���� ����� ������� ��������, �� ��������� �� � ����� ����
	
	//iPoint point = iPoint(420, 260);
	//if (IsPointInPoligon(secondPoligon, point, true)) {
	//	//	��������� ������� 10 �� 10 �������� � ������� � ����� point � ������� ����
	//	for (int x = point.x - 5; x <= point.x + 5; x++)
	//	{
	//		for (int y = point.y - 5; y <= point.y + 5; y++)
	//		{
	//			gfSetPixel(x, y, RGBPIXEL(255, 0, 0));
	//		}
	//	}
	//}
	//else {
	//	//	��������� ������� 10 �� 10 �������� � ������� � ����� point � ����� ����
	//	for (int x = point.x - 5; x <= point.x + 5; x++)
	//	{
	//		for (int y = point.y - 5; y <= point.y + 5; y++)
	//		{
	//			gfSetPixel(x, y, RGBPIXEL(0, 0, 255));
	//		}
	//	}
	//}

	// ������� 4 �����
	iPoint point1 = iPoint(100, 100);
	iPoint point2 = iPoint(200, 100);
	iPoint point3 = iPoint(150, 200);
	iPoint point4 = iPoint(150, 20);
	
	if (IsIntersect(point1, point2, point3, point4)) {

		gfDrawText(200, 200, "hello world", RGBPIXEL(0, 128, 255));
	}
	
	
	

	
	DrawOwnPoligon(firstPoligon, RGBPIXEL(255, 0, 0));
	DrawOwnPoligon(secondPoligon, RGBPIXEL(0, 255, 0));

	ColorPoligon(firstPoligon, RGBPIXEL(255, 0, 0), true);
	ColorPoligon(secondPoligon, RGBPIXEL(0, 255, 0), false);

	/*gfsetpixel( 20, 20, rgbpixel(255, 255, 0) );*/

	/*gfdrawrectangle( 100, 120, 170, 150, rgbpixel(255, 255, 0) );

	

	drawline(10, 10, 500, 300, rgbpixel::green());
	*/

	return true;
}


// ���������� � ����� �� ������� ������ �� ����������.
// ������� ������������ ��� �������� ������������ ��������
void gfDrawScene()
{
    //gfClearScreen(RGBPIXEL::Black());

    //static int x = 0;
    //gfDrawRectangle(x, 100, x + 50, 130, RGBPIXEL::Blue());
    //x = (x + 1) % gfGetWindowWidth() ;

    //int x = gfGetMouseX(),
    //    y = gfGetMouseY();
    //gfDrawRectangle(x - 10, y - 10, x + 10, y + 10, RGBPIXEL::Green());
}

// ���������� ���� ��� ����� ������� �� ����������.
// ������� ������������ ��� ������������ ����������
// �������� (������, ������ � �.�.)
void gfCleanupScene()
{
}

// ���������� ����� ������������ �������� ����� ������ ����
void gfOnLMouseClick( int x, int y )
{
    x; y;
    gfDrawRectangle(x - 10, y - 10, x + 10, y + 10, RGBPIXEL::Green());
}

// ���������� ����� ������������ �������� ������ ������ ����
void gfOnRMouseClick( int x, int y )
{
    x; y;
}

// ���������� ����� ������������ �������� ������� �� ����������
void gfOnKeyDown( UINT key )
{
    key;

    if( key == 'A' )
        gfDisplayMessage( "'A' key has been pressed" );
}

// ���������� ����� ������������ �������� ������� �� ����������
void gfOnKeyUp( UINT key )
{
    key;

    //if( key == 'B' )
    //    gfDisplayMessage( "'B' key has been un-pressed" );
}
