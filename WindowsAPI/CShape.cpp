
#define _USE_MATH_DEFINES

#include "CShape.h"
#include <cmath>

Point Rotate(const Point &center, const Point &p, const double radian);

void CShape::Update(const RECT& rt, int f)
{
	flag = f;
	Collision(rt);
	pos.x += x;
	pos.y += y;
}

void CCircle::Collision(const RECT& rt)
{
	if (rt.right <= pos.x + R)
	{
		//Direction = 2  *(M_PI - Direction);
//		Direction += 3 * M_PI / 2;
		x *= -1;
		if (rt.right < pos.x + R)
			pos.x = rt.right - R;
	}
	if (rt.left >= pos.x - R)
	{
		//Direction = 2 * (M_PI - Direction);
//		Direction += M_PI/ 2;
		x *= -1; 
		if(rt.left > pos.x - R)
			pos.x = rt.left + R + 1;
	}
	if (rt.top >= pos.y - R)
	{
		y *= -1;
//		Direction += M_PI;
//		Direction += M_PI / 2;

		if (rt.top > pos.y - R)
			pos.y = rt.top + R +1 ;
	}
	if (rt.bottom <= pos.y + R)
	{
		y *= -1;
//		Direction += 3 * M_PI / 2;
		if (rt.bottom < pos.y + R)
			pos.y = rt.bottom - R;
	}
}

void CCircle::Collision(std::vector<CShape *> &v, int &i, int &j)
{
	CCircle *c = dynamic_cast<CCircle *>(v[j]);

	if (c)
	{
		FLOAT len = sqrt(pow(this->pos.x - c->pos.x, 2) + pow(this->pos.y - c->pos.y, 2));
		if (R + c->R >= len)
		{
			FLOAT vx = -1 * (this->pos.x - c->pos.x) / len, vy = -1 * (this->pos.y - c->pos.y) / len;	// 법선벡터 단위벡터
			FLOAT inner_this_tan = x * -1.0 * vy + y * vx; // 접선과 this벡터 내적
			FLOAT inner_this_nor = vx * x + vy * y;		// 법선과 this벡터 내적
			FLOAT inner_c_tan = c->x * -1.0 * vy + c->y * vx; // 접선과 c벡터 내적
			FLOAT inner_c_nor = vx * c->x + vy * c->y;	// 법선과 c벡터 내적
			
			x = vx * (inner_c_nor)+(-1.0) * vy * (inner_this_tan)+0.5;
			y = vy * (inner_c_nor)+(-1.0) * vx * (inner_this_tan)+0.5;

			c->x = vx * (inner_this_nor)+(-1.0) * vy * (inner_c_tan)+0.5;
			c->y = vy * (inner_this_nor)+(-1.0) * vx * (inner_c_tan)+0.5;

				
			FLOAT d = R + c->R - len;
			if (d > 0)
			{
				d *= 0.5;
				pos.x += d * vx* -1;
				pos.y += d * vy* -1;
				c->pos.x += d * vx;
				c->pos.y += d * vy;
			}

		}

	}

	CRectangle *r = dynamic_cast<CRectangle *>(v[j]);

	if (r)
		r->Collision(v, j, i);

	CStar *s = dynamic_cast<CStar *>(v[j]);

	if (s)
		s->Collision(v, j, i);

}

void CCircle::Show(HDC &hdc)
{
	Ellipse(hdc, pos.x - R, pos.y - R, pos.x +R, pos.y + R);
}

void CRectangle::Collision(const RECT &rt)
{
	FLOAT left = pos.x - (width * 0.5);
	FLOAT right = pos.x + (width * 0.5);
	FLOAT top = pos.y - (height * 0.5);
	FLOAT bottom = pos.y + (height * 0.5);
	POINT p1[4] = { left, right, top, bottom };
	Point p2[4][2] = { {{left,top}, {right, top}},{{right, top}, {right, bottom}}, { {right, bottom}, {left, bottom}}, {{left, bottom}, {left,top}} };

	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 2; j++)
			p2[i][j] = Rotate(pos, p2[i][j], Radian);
	}

	FLOAT diagonal = sqrt((width * 0.5) * (width * 0.5) + (height * 0.5) * (height * 0.5));
	FLOAT len;
	FLOAT l, r, t, b;
	for (int i = 0; i < 4; i++)
	{
		
		(p2[i][0].x > p2[i][1].x) ? (r = p2[i][0].x, l = p2[i][1].x) : (l = p2[i][0].x, r = p2[i][1].x);
		(p2[i][0].y > p2[i][1].y) ? (b = p2[i][0].y, t = p2[i][1].y) : (t = p2[i][0].y, b = p2[i][1].y);

		if (l <= rt.left)
		{
			x *= -1;

			pos.x = rt.left + (pos.x - l);
			break;
		}
		if (r >= rt.right)
		{
			x *= -1;
			pos.x = rt.right - (r- pos.x);
			break;
		}
		if (t <= rt.top)
		{
			y *= -1;
			pos.y = rt.top + (pos.y - t);
			break;
		}
		if (b >= rt.bottom)
		{
			y *= -1;
			pos.y = rt.bottom - (b - pos.y);
			break;
		}
	}



}

void CRectangle::Collision(std::vector<CShape*> &v, int &i, int &j)
{
	CRectangle *r = dynamic_cast<CRectangle *>(v[j]);
	
	if (r)
	{
		bool ch = false;

		FLOAT left = pos.x - (width * 0.5);
		FLOAT right = pos.x + (width * 0.5);
		FLOAT top = pos.y - (height * 0.5);
		FLOAT bottom = pos.y + (height * 0.5);
		Point p1[4] = { {left,top},{right, top}, {right, bottom}, {left, bottom} };

		for (int i = 0; i < 4; i++)
		{
			p1[i] = Rotate(pos, p1[i], Radian);
		}

		left = r->pos.x - (r->width * 0.5);
		right = r->pos.x + (r->width * 0.5);
		top = r->pos.y - (r->height * 0.5);
		bottom = r->pos.y + (r->height * 0.5);
		Point p2[4] = { {left,top},{right, top}, {right, bottom}, {left, bottom} };

		for (int i = 0; i < 4; i++)
		{
			p2[i] = Rotate(r->pos, p2[i], r->Radian);
		}

		FLOAT v1x = p1[1].x - p1[0].x, v1y = p1[1].y - p1[0].y;
//		FLOAT v2x = -1 * v1y, v2y = v1x;
		FLOAT v2x = p1[3].x - p1[0].x, v2y = p1[3].y - p1[0].y;
		FLOAT pv1 = sqrt(pow(v1x, 2) + pow(v1y, 2));
		FLOAT pv2 = sqrt(pow(v2x, 2) + pow(v2y, 2));
		v1x /= pv1; v1y /= pv1;
		v2x /= pv2; v2y /= pv2;
		

		for (int i = 0; i < 4; i++)
		{
			FLOAT dx = p2[i].x - p1[0].x, dy = p2[i].y - p1[0].y;

			FLOAT innerx = dx * v1x + dy * v1y;
			FLOAT innery = dx * v2x + dy * v2y;

			if (innerx >= 0 && innerx <= width&& innery >= 0 && innery <= width)
			{
				ch = true;
				break;
			}

		}


		if (!ch)
		{
			v1x = p2[1].x - p2[0].x, v1y = p2[1].y - p2[0].y;
			//v2x = -1 * v1y, v2y = v1x;
			v2x = p2[3].x - p2[0].x, v2y = p2[3].y - p2[0].y;
			pv1 = sqrt(pow(v1x, 2) + pow(v1y, 2));
			pv2 = sqrt(pow(v2x, 2) + pow(v2y, 2));
			v1x /= pv1; v1y /= pv1;
			v2x /= pv2; v2y /= pv2;

			for (int i = 0; i < 4; i++)
			{
				FLOAT dx = p1[i].x - p2[0].x, dy = p1[i].y - p2[0].y;

				FLOAT innerx = dx * v1x + dy * v1y;
				FLOAT innery = dx * v2x + dy * v2y;

				if (innerx >= 0 && innerx <= width && innery >= 0 && innery <= width)
				{
					ch = true;
					break;
				}

			}
		}

		if (ch)
		{
			FLOAT nx, ny;
			FLOAT tx, ty;
			nx = pos.x - r->pos.x; ny = pos.y - r->pos.y;
			FLOAT p = sqrt(pow(nx, 2) + pow(ny, 2));
			nx /= p; ny /= p;

			tx = -1 * ny;
			ty = nx;

			//normal inner
			FLOAT nor_inner1 = x * nx + y * ny;
			FLOAT nor_inner2 = r->x * nx + r->y * ny;
			//tangent inner
			FLOAT tan_inner1 = x * tx + y * ty;
			FLOAT tan_inner2 = r->x * tx + r->y * ty;

			x = nx * nor_inner2 + tx * tan_inner1;
			y = ny * nor_inner2 + ty * tan_inner1;

			r->x = nx * nor_inner1 + tx * tan_inner2;
			r->y = ny * nor_inner1 + ty * tan_inner2;

			FLOAT len = sqrt(pow(pos.x - r->pos.x, 2) + pow(pos.y - r->pos.y, 2));
			FLOAT d = ((width * sqrt(2)*0.5 + r->width * sqrt(2)*0.5) - len) * 0.5;

//			pos.x += d * nx;
//			pos.y += d * ny;
//			r->pos.x -= d * nx;
//			r->pos.y -= d * ny;

		}



	}

	CCircle *c = dynamic_cast<CCircle *>(v[j]);

	if (c)
	{
		FLOAT left = pos.x - (width * 0.5);
		FLOAT right = pos.x + (width * 0.5);
		FLOAT top = pos.y - (height * 0.5);
		FLOAT bottom = pos.y + (height * 0.5);
		Point p[4][2] = { {{left,top}, {right, top}},{{right, top}, {right, bottom}}, { {right, bottom}, {left, bottom}}, {{left, bottom}, {left,top}} };

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
				p[i][j] = Rotate(pos, p[i][j], Radian);
		}

		FLOAT v1x, v1y, v2x, v2y, v3x,v3y;
		bool ch = false;
		Point cp = c->getp();
		FLOAT dis = sqrt((pos.x - cp.x, 2) + pow(pos.y- cp.y, 2));
		if (dis > c->getR() + width * 0.5 * sqrt(2))
			return;

		for (int i = 0; i < 4; i++)
		{
			v1x = p[i][1].x - p[i][0].x;
			v1y = p[i][1].y - p[i][0].y;
			v2x = cp.x - p[i][0].x;
			v2y = cp.y - p[i][0].y;

			v1x /= width;
			v1y /= width;

			FLOAT inner = v1x * v2x + v1y * v2y;

			v3x = v2x - v1x * inner;
			v3y = v2y - v1y * inner;
			dis = sqrt(pow(v3x, 2) + pow(v3y, 2));

			if (inner >= 0 && inner <= width + c->getR() && dis <= c->getR())
			{
				if (inner > width)
				{
					dis = sqrt(pow(p[i][1].x - cp.x, 2) + pow(p[i][1].y - cp.y, 2));
					if(dis <= c->getR())
						ch = true;
					else
						ch = false;

				}
				else
					ch = true;
				
			}
			if (ch)
				break;



		}

		if (ch)
		{
			switch (flag)
			{
			case 1:
			{
				FLOAT nx, ny;
				FLOAT tx, ty;
				nx = pos.x - cp.x; ny = pos.y - cp.y;
				FLOAT p = sqrt(pow(nx, 2) + pow(ny, 2));
				nx /= p; ny /= p;

				tx = -1 * ny;
				ty = nx;

				//normal inner
				FLOAT nor_inner1 = x * nx + y * ny;
				FLOAT nor_inner2 = c->getx() * nx + c->gety() * ny;
				//tangent inner
				FLOAT tan_inner1 = x * tx + y * ty;
				FLOAT tan_inner2 = c->getx() * tx + c->gety() * ty;

				x = nx * nor_inner2 + tx * tan_inner1;
				y = ny * nor_inner2 + ty * tan_inner1;


				c->setx(nx * nor_inner1 + tx * tan_inner2);
				c->sety(ny * nor_inner1 + ty * tan_inner2);
				//			


				//			FLOAT d2 = c->getR() - 

				FLOAT len = sqrt(pow(pos.x - cp.x, 2) + pow(pos.y - cp.y, 2));
				FLOAT d = ((width * 0.5 * sqrt(2) + c->getR()) - len) * 0.5;
//				pos.x += d * nx;
//				pos.y += d * ny;
//				cp.x += (c->getx() - d * nx);
//				cp.y += (c->gety() - d * ny);
//				c->setp(cp);


			}
				break;
			case 2:
			{
				width *= 2;
				height *= 2;
				c->_delete = true;
				if (width >= 800)
				{
					_delete = true;
				}
			}
			break;
			case 3:
			{
				width /= 2;
				height /= 2;
				c->_delete = true;
				if (width <= 12)
				{
					_delete = true;
				}
			}
			break;
			}
		}






	}

	CStar *s = dynamic_cast<CStar *>(v[j]);

	if (s)
	{
		

		FLOAT left = pos.x - (width * 0.5);
		FLOAT right = pos.x + (width * 0.5);
		FLOAT top = pos.y - (height * 0.5);
		FLOAT bottom = pos.y + (height * 0.5);
		FLOAT p[4][2][2] = { {{left,top}, {right, top}},{{right, top}, {right, bottom}}, { {right, bottom}, {left, bottom}}, {{left, bottom}, {left,top}} };
		FLOAT v1x, v1y, v2x, v2y, v3x, v3y;
		bool ch = false;
		Point cp = s->getp();
		FLOAT dis = sqrt((pos.x - cp.x, 2) + pow(pos.y - cp.y, 2));
		if (dis > s->getR() + width * 0.5 * sqrt(2))
			return;

		for (int i = 0; i < 4; i++)
		{
			v1x = p[i][1][0] - p[i][0][0];
			v1y = p[i][1][1] - p[i][0][1];
			v2x = cp.x - p[i][0][0];
			v2y = cp.y - p[i][0][1];

			v1x /= width;
			v1y /= width;

			FLOAT inner = v1x * v2x + v1y * v2y;

			v3x = v2x - v1x * inner;
			v3y = v2y - v1y * inner;
			dis = sqrt(pow(v3x, 2) + pow(v3y, 2));

			if (inner >= 0 && inner <= width + s->getR() && dis <= s->getR())
			{
				if (inner > width)
				{
					dis = sqrt(pow(p[i][1][0] - cp.x, 2) + pow(p[i][1][1] - cp.y, 2));
					if (dis <= s->getR())
						ch = true;
					else
						ch = false;

				}
				else
					ch = true;

			}
			if (ch)
				break;



		}

		if (ch)
		{
			switch (flag)
			{
			case 1:
			{
				FLOAT nx, ny;
				FLOAT tx, ty;
				nx = pos.x - cp.x; ny = pos.y - cp.y;
				FLOAT p = sqrt(pow(nx, 2) + pow(ny, 2));
				nx /= p; ny /= p;

				tx = -1 * ny;
				ty = nx;

				//normal inner
				FLOAT nor_inner1 = x * nx + y * ny;
				FLOAT nor_inner2 = s->getx() * nx + s->gety() * ny;
				//tangent inner
				FLOAT tan_inner1 = x * tx + y * ty;
				FLOAT tan_inner2 = s->getx() * tx + s->gety() * ty;

				x = nx * nor_inner2 + tx * tan_inner1;
				y = ny * nor_inner2 + ty * tan_inner1;


				s->setx(nx * nor_inner1 + tx * tan_inner2);
				s->sety(ny * nor_inner1 + ty * tan_inner2);
				//			


				//			FLOAT d2 = c->getR() - 

				FLOAT len = sqrt(pow(pos.x - cp.x, 2) + pow(pos.y - cp.y, 2));
				FLOAT d = ((width * 0.5 * sqrt(2) + s->getR()) - len) * 0.5;
				pos.x += d * nx;
				pos.y += d * ny;
				cp.x += (s->getx() - d * nx);
				cp.y += (s->gety() - d * ny);

				s->setp(cp);

			}
			break;
			case 2:
			{
				s->setR(s->getR() * 2);
				if (s->getR() >= 400)
				{
					s->_delete = true;
				}
				_delete = true;

			}
			break;
			case 3:
			{
				s->setR(s->getR() / 2.0);
				if (s->getR() <= 6)
				{
					s->_delete = true;
				}
				_delete = true;
			}
			break;
			}
		}

	}
}


void CRectangle::Show(HDC &hdc)
{
	int left = pos.x - (width * 0.5);
	int right = pos.x + (width * 0.5);
	int top = pos.y - (height * 0.5);
	int bottom = pos.y + (height * 0.5);
	Point point[4] = { {left,top}, {right, top},{right, bottom}, {left, bottom} };
	POINT p[4];

	for (int i = 0; i < 4; i++)
	{
		point[i] = Rotate(pos, point[i], Radian);
		p[i].x = point[i].x;
		p[i].y = point[i].y;
	}
	
	Polygon(hdc, p, 4);
}

void CStar::Collision(const RECT &rt)
{
	if (rt.right <= pos.x + R)
	{
		x *= -1;
		if (rt.right < pos.x + R)
			pos.x = rt.right - R;
	}
	if (rt.left >= pos.x - R)
	{
		x *= -1;
		if (rt.left > pos.x - R)
			pos.x = rt.left + R + 1;
	}
	if (rt.top >= pos.y - R)
	{
		y *= -1;

		if (rt.top > pos.y - R)
			pos.y = rt.top + R + 1;
	}
	if (rt.bottom <= pos.y + R)
	{
		y *= -1;
		if (rt.bottom < pos.y + R)
			pos.y = rt.bottom - R;
	}
}

void CStar::Collision(std::vector<CShape*>& v, int &i, int &j)
{
	CStar *s = dynamic_cast<CStar *>(v[j]);

	if (s)
	{
		FLOAT len = sqrt(pow(this->pos.x - s->pos.x, 2) + pow(this->pos.y - s->pos.y, 2));
		if (R + s->R >= len)
		{
			FLOAT vx = -1 * (this->pos.x - s->pos.x) / len, vy = -1 * (this->pos.y - s->pos.y) / len;	// 법선벡터 단위벡터
			FLOAT inner_this_tan = x * -1.0 * vy + y * vx;	// 접선과 this벡터 내적
			FLOAT inner_this_nor = vx * x + vy * y;			// 법선과 this벡터 내적
			FLOAT inner_c_tan = s->x * -1.0 * vy + s->y * vx; // 접선과 c벡터 내적
			FLOAT inner_c_nor = vx * s->x + vy * s->y;	// 법선과 c벡터 내적

			x = vx * (inner_c_nor)+(-1.0) * vy * (inner_this_tan);
			y = vy * (inner_c_nor)+(-1.0) * vx * (inner_this_tan);

			s->x = vx * (inner_this_nor)+(-1.0) * vy * (inner_c_tan);
			s->y = vy * (inner_this_nor)+(-1.0) * vx * (inner_c_tan);


			FLOAT d = R + s->R - len;
			if (d > 0)
			{
				d *= 0.5;
				pos.x += d * vx* -1;
				pos.y += d * vy* -1;
				s->pos.x += d * vx;
				s->pos.y += d * vy;
			}

		}

	}

	CCircle *c = dynamic_cast<CCircle *>(v[j]);

	if (c)
	{
		
			FLOAT len = sqrt(pow(this->pos.x - c->pos.x, 2) + pow(this->pos.y - c->pos.y, 2));
			if (R + c->getR() >= len)
			{
				switch (flag)
				{
				case 1:
				{
				FLOAT vx = -1 * (this->pos.x - c->pos.x) / len, vy = -1 * (this->pos.y - c->pos.y) / len;	// 법선벡터 단위벡터
				FLOAT inner_this_tan = x * -1.0 * vy + y * vx; // 접선과 this벡터 내적
				FLOAT inner_this_nor = vx * x + vy * y;		// 법선과 this벡터 내적
				FLOAT inner_c_tan = c->x * -1.0 * vy + c->y * vx; // 접선과 c벡터 내적
				FLOAT inner_c_nor = vx * c->x + vy * c->y;	// 법선과 c벡터 내적

				x = vx * (inner_c_nor)+(-1.0) * vy * (inner_this_tan);
				y = vy * (inner_c_nor)+(-1.0) * vx * (inner_this_tan);

				c->x = vx * (inner_this_nor)+(-1.0) * vy * (inner_c_tan);
				c->y = vy * (inner_this_nor)+(-1.0) * vx * (inner_c_tan);


				FLOAT d = R + c->getR() - len;
				if (d > 0)
				{
					d *= 0.5;
					pos.x += d * vx* -1;
					pos.y += d * vy* -1;
					c->pos.x += d * vx;
					c->pos.y += d * vy;
				}

				}
				break;
				case 2:
				{
					R *= 2;
					c->_delete = true;
					if (R >= 400)
					{
						_delete = true;
					}
				}
				break;
				case 3:
				{
					R /= 2.0;
					c->_delete = true;
					if (R <= 6)
					{
						_delete = true;
					}
				}
			}

		}


	}

	CRectangle *r = dynamic_cast<CRectangle *>(v[j]);

	if (r)
		r->Collision(v, j, i);
}

void CStar::Show(HDC &hdc)
{
	if (R == 0) return;
	FLOAT radian = 2 * M_PI / 5;
	Point point[10];
	POINT p[10];
	FLOAT x = pos.x, y = pos.y, r = R * cos(radian) / cos(radian / 2);
	for (int i = 0; i < 5; i++)
	{
		point[i * 2].x = pos.x + R * sin(radian * i);
		point[i * 2].y = pos.y - R * cos(radian * i);
		point[i * 2 + 1].x = pos.x + r * sin((radian / 2) + radian * i);
		point[i * 2 + 1].y = pos.y - r * cos((radian / 2) + radian * i);
	}

	for (int i = 0; i < 5; i++)
	{
		point[i * 2] = Rotate(pos, point[i * 2], Radian);
		point[i * 2 + 1] = Rotate(pos, point[i * 2 + 1], Radian);
	}
	for (int i = 0; i < 5; i++)
	{
		p[i * 2].x = LONG(point[i * 2].x);
		p[i * 2].y = LONG(point[i * 2].y);
		p[i * 2 + 1].x = LONG(point[i * 2 + 1].x);
		p[i * 2 + 1].y = LONG(point[i * 2 + 1].y);
	}

	Polygon(hdc, p, 10);
}


Point Rotate(const Point &center, const Point &p, const double radian)
{
	Point temp = { p.x - center.x, p.y - center.y };

	FLOAT tempx = cos(radian) * temp.x - sin(radian) * temp.y;
	FLOAT tempy = sin(radian) * temp.x + cos(radian) * temp.y;

	return { tempx + center.x, tempy + center.y };
}