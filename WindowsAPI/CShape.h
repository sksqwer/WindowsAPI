#pragma once
#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>
#include "framework.h"
#include "WindowsAPI.h"

struct Point
{
	FLOAT x;
	FLOAT y;
};

class CShape
{
protected:
public:
	Point pos;
	FLOAT x;
	FLOAT y;
	int Type;
	enum { Circle, Rectangle, Star};
	FLOAT Radian = 0;
	int flag = 1;
	bool _delete = false;

	CShape(Point p, FLOAT x, FLOAT y, int t)
		: pos(p), x(x), y(y), Type(t) {};
	void Update(const RECT&, int);
	Point getp() { return pos; }
	FLOAT getx() { return x; }
	FLOAT gety() { return y; }
	void setx(FLOAT x) { this->x = x; }
	void sety(FLOAT y) { this->y = y; }
	void setp(Point p) { pos = p; }
	virtual void Collision(const RECT&) {}
	virtual void Collision(std::vector<CShape *>&, int&, int&) {}
	virtual void Show(HDC &) {}
};

class CCircle : public CShape
{
private:
	FLOAT R;
public:
	CCircle(Point p, FLOAT x, FLOAT y, FLOAT r, int t = CShape::Circle)
		: CShape(p, x, y, t), R(r){}
	void Collision(const RECT&);
	void Collision(std::vector<CShape *>&, int& ,int&);
	void Show(HDC &);
	FLOAT getR() { return R; }
};

class CRectangle : public CShape
{
private:
	FLOAT width;
	FLOAT height;
public:
	CRectangle(Point p, FLOAT x, FLOAT y, FLOAT w, FLOAT h, int t = CShape::Rectangle)
		: CShape(p, x, y, t), width(w), height(h) {}
	void Collision(const RECT&);
	void Collision(std::vector<CShape *>&, int&, int&);
	FLOAT getw() { return width; }
	void Show(HDC &);
};

class CStar : public CShape
{
private:
	FLOAT R;
public:
	CStar(Point p, FLOAT x, FLOAT y, FLOAT r, int t = CShape::Star)
		: CShape(p, x, y, t), R(r) {}
	void Collision(const RECT&);
	void Collision(std::vector<CShape *>&, int&, int&);
	void Show(HDC &);
	FLOAT getR() { return R; }
	void setR(FLOAT r) { R = r; }
};
#endif // !SHAPE_H_
