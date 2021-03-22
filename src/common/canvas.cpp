#include "canvas.h"
#include "Log.h"
#include <cassert>
#include <algorithm>

using namespace std;

canvas::canvas(int w, int h) 
	: m_Canvas({ w, h })
{
}

canvas::~canvas()
{
}

void canvas::save(const std::string& filename)
{
   save_png(m_Canvas, filename.c_str());
}

void canvas::begin(PrimitiveType type)
{
	m_DrawType = type;
}

void canvas::end()
{
	switch (m_DrawType)
	{
		case LINES:
		{
			draw_lines(); break;
		}
		case TRIANGLES:
		{
			draw_triangles(); break;
		}
		case CIRCLES:
		{
			draw_circles(); break;
		}
	}
	m_DrawType = PrimitiveType::UNDEFINED_TYPE;
	m_Vertices.clear();
}

void canvas::vertex(int x, int y)
{
	m_Vertices.push_back(Vertex({ x, y, m_Color }));
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
	m_Color = { r, g, b };
}

// currently overwrites canvas
void canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
	m_Background = { r, g, b };
	for (int i = 0; i < m_Canvas.width(); i++)
		for (int j = 0; j < m_Canvas.height(); j++)
			m_Canvas.set(i, j, m_Background);
}

void canvas::draw_lines()
{
	for (int i = 0; i < m_Vertices.size(); i+=2)
	{
		Vertex v0 = m_Vertices[i];
		Vertex v1 = m_Vertices[i+1];

		if (abs(v1.y - v0.y) < abs(v1.x - v0.x))
		{
			if (v0.x > v1.x)
				draw_line_low(v1, v0);
			else
				draw_line_low(v0, v1);
		}
		else
		{
			if (v0.y > v1.y)
				draw_line_high(v1, v0);
			else
				draw_line_high(v0, v1);
		}
	}
}

void canvas::draw_line_low(Vertex v0, Vertex v1)
{
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;

	// 'slope' of the color
	Pixel c0 = v0.color;
	Pixel c1 = v1.color;
	float dc[3] = { 0, 0, 0 };
	if (dx + dy)
	{
		dc[0] = (c1.r - c0.r) / (dx + dy);
		dc[1] = (c1.g - c0.g) / (dx + dy);
		dc[2] = (c1.b - c0.b) / (dx + dy);
	}
	int yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}

	int D = (2 * dy) - dx;
	int y = v0.y;

	for (int x = v0.x; x < v1.x; x++)
	{
		uint8_t r = float(dc[0] * (x - v0.x + y - v0.y) + c0.r);
		uint8_t g = float(dc[1] * (x - v0.x + y - v0.y) + c0.g);
		uint8_t b = float(dc[2] * (x - v0.x + y - v0.y) + c0.b);
		if (x >= 0 && x < m_Canvas.width() && y >= 0 && y < m_Canvas.height())
			m_Canvas.set(x, y, { r, g, b });
		if (D > 0)
		{
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else
			D = D + 2 * dy;
	}
}

void canvas::draw_line_high(Vertex v0, Vertex v1)
{
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;

	// 'slope' of the color
	Pixel c0 = v0.color;
	Pixel c1 = v1.color;

	float dc[3] = { 0.0f, 0.0f, 0.0f };
	if (dx + dy)
	{
		dc[0] = (float)(c1.r - c0.r) / (dx + dy);
		dc[1] = (float)(c1.g - c0.g) / (dx + dy);
		dc[2] = (float)(c1.b - c0.b) / (dx + dy);
	}

	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = (2 * dx) - dy;
	int x = v0.x;

	for (int y = v0.y; y < v1.y; y++)
	{
		uint8_t r = float(dc[0] * (x - v0.x + y - v0.y) + c0.r);
		uint8_t g = float(dc[1] * (x - v0.x + y - v0.y) + c0.g);
		uint8_t b = float(dc[2] * (x - v0.x + y - v0.y) + c0.b);
		if (x >= 0 && x < m_Canvas.width() && y >= 0 && y < m_Canvas.height())
			m_Canvas.set(x, y, { r, g, b });
		if (D > 0)
		{
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else
			D = D + 2 * dx;
	}
}

// I was having some issues with this one :(
//void canvas::draw_triangles()
//{
//	for (int i = 2; i < m_Vertices.size(); i += 3)
//	{
//		Vertex a = m_Vertices[i-2];
//		Vertex b = m_Vertices[i-1];
//		Vertex c = m_Vertices[i];
//
//		// color
//		Pixel ca = a.color;
//		Pixel cb = b.color;
//		Pixel cc = c.color;
//
//		// area
//		float area = abs(glm::determinant(glm::mat2{
//				b.x - a.x, c.x - a.x,
//				b.y - a.y, c.y - a.y
//			}));
//
//		// bounding box
//		int x0 = min(min(a.x, b.x), c.x);
//		int y0 = min(min(a.y, b.y), c.y);
//		int x1 = max(max(a.y, b.y), c.y);
//		int y1 = max(max(a.y, b.y), c.y);
//
//		for (int x = x0; x < x1; x++)
//		{
//			for (int y = y0; y < y1; y++)
//			{
//				// barycentric coords
//				float beta = abs(glm::determinant(glm::mat2{
//						a.x - c.x, x - c.x,
//						a.y - c.y, y - c.y
//					})) / area;
//				float gamma = abs(glm::determinant(glm::mat2{
//						b.x - a.x, x - a.x,
//						b.y - a.y, y - a.y
//					})) / area;
//				float alpha = abs(glm::determinant(glm::mat2{
//						c.x - b.x, x - b.x,
//						c.y - b.y, y - b.y,
//					})) / area;
//				
//				if (alpha + beta + gamma <= 1)
//				{
//					Pixel color = {
//						(uint8_t)float(alpha * ca.r + beta * cb.r + gamma * cc.r),
//						(uint8_t)float(alpha * ca.g + beta * cb.g + gamma * cc.g),
//						(uint8_t)float(alpha * ca.b + beta * cb.b + gamma * cc.b),
//						255
//					};
//					if (color.r + color.g + color.b <= 3 * 50 && y < 200)
//						CORE_ASSERT(false, "problem");
//					m_Canvas.set(x, y, color);
//				}
//			}
//		}
//
//	}
//}

void canvas::draw_triangles()
{
	for (int i = 2; i < m_Vertices.size(); i += 3)
	{
		Vertex a = m_Vertices[i - 2];
		Vertex b = m_Vertices[i - 1];
		Vertex c = m_Vertices[i];

		// color
		Pixel ca = a.color;
		Pixel cb = b.color;
		Pixel cc = c.color;

		// bounding box
		int xmin = min(min(a.x, b.x), c.x);
		int ymin = min(min(a.y, b.y), c.y);
		int xmax = max(max(a.y, b.y), c.y);
		int ymax = max(max(a.y, b.y), c.y);

		//float f_alpha = f_bc(a);
		//float f_beta = f_ac(b);
		//float f_gamma = f_ab(c);

		float f_alpha = (c.y - b.y) * (a.x - b.x) - (c.x - b.x) * (a.y - b.y);
		float f_beta = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y);
		float f_gamma = (b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y);

		for (int y = ymin; y < ymax; y++)
		{
			for (int x = xmin; x < xmax; x++)
			{
				float alpha =float ( (c.y - b.y) * (x - b.x) - (c.x - b.x) * (y - b.y)) / f_alpha;
				float beta = float ((c.y - a.y) * (x - a.x) - (c.x - a.x) * (y - a.y) )/ f_beta;
				float gamma =float ( (b.y - a.y) * (x - a.x) - (b.x - a.x) * (y - a.y)) / f_gamma;
				if (alpha >= 0 && beta >= 0 && gamma >= 0)
				{
					Pixel color = {
						(uint8_t)float(alpha * ca.r + beta * cb.r + gamma * cc.r),
						(uint8_t)float(alpha * ca.g + beta * cb.g + gamma * cc.g),
						(uint8_t)float(alpha * ca.b + beta * cb.b + gamma * cc.b),
						255
					};
					if (alpha > 0 || f_alpha * (c.y - b.y) * (-1 - b.x) - (c.x - b.x) * (-1 - b.y)) m_Canvas.set(x, y, color);
					if (alpha > 0 || f_beta * (c.y - a.y) * (-1 - a.x) - (c.x - a.x) * (-1 - a.y)) m_Canvas.set(x, y, color);
					if (alpha > 0 || f_gamma * (b.y - a.y) * (-1 - a.x) - (b.x - a.x) * (-1 - a.y)) m_Canvas.set(x, y, color);
				}
			}
		}
	}
}

void canvas::draw_circles()
{
	for (int i = 0; i < m_Vertices.size(); i += 2)
	{
		Vertex a = m_Vertices[i];
		Vertex b = m_Vertices[i+1];

		// color 
		Pixel ca = a.color;
		Pixel cb = b.color;

		float radius = glm::distance(glm::vec2(b.x, b.y), glm::vec2(a.x, a.y));
		
		// bounding box
		int x0 = abs(a.x - radius);
		int x1 = abs(a.x + radius);
		int y0 = abs(a.y - radius);
		int y1 = abs(a.y + radius);

		for (int x = x0; x < x1; x++)
		{
			for (int y = y0; y < y1; y++)
			{
				float dist = glm::distance(glm::vec2(a.x, a.y), glm::vec2(x, y));
				if (dist <= radius)
				{
					Pixel color = {
						(uint8_t)float((radius - dist) / radius * ca.r + dist / radius * cb.r),
						(uint8_t)float((radius - dist) / radius * ca.g + dist / radius * cb.g),
						(uint8_t)float((radius - dist) / radius * ca.b + dist / radius * cb.b)
					};
					m_Canvas.set(x, y, color);
				}
			}
		}

	
	}
}
