#include <iostream>
#include <functional>
#include "canvas.h"
#include "Log.h"

void draw_parametric(canvas& c, std::function<glm::vec2(float t)> param, float t0, float t1, float resolution)
{
	c.begin(LINES);
	
	glm::vec2 point = param(t0);
	for (float t = t0; t < t1;)
	{
		c.vertex(point.x, point.y);
		t += 1 / resolution;
		point = param(t);
		c.vertex(point.x, point.y);
	}

	c.end();
}

// parametric functions
void draw_circle(canvas& c, glm::vec2 center, float radius, float resolution)
{
	draw_parametric(c, [=](float t) {
		return glm::vec2(glm::cos(t) * radius + center.x, glm::sin(t) * radius + center.y);
		},
		0, 2 * 3.1416, resolution
	);
	
}

void draw_box_curve(canvas& c, glm::vec2 center, float radius, float ratio, float t0, float t1, float resolution)
{
	draw_parametric(c, [=](float t) {
		return glm::vec2(glm::cos(t * ratio) * radius + center.x, glm::sin(t * 1/ ratio) * radius + center.y);
		},
		t0, t1, resolution
	);
}

int main(int argc, char** argv)
{
	Log::Init("Homework2");
	canvas drawer(600, 600);
	// your code here

	// lines
	//drawer.begin(LINES);

	//for (int i = 50; i < 550; i+=10)
	//{
	//	drawer.color(0,0,255);
	//	drawer.vertex(i,50);
	//	drawer.color(0, 0, 0);
	//	drawer.vertex(i,550);
	//}
	//drawer.end();


	// triangles
	drawer.begin(TRIANGLES);
	//t1
	drawer.color(0,255,0);
	drawer.vertex(100, 100);
	drawer.vertex(500, 100);
	drawer.color(0, 0, 0);
	drawer.vertex(100, 500);
	//t2
	drawer.vertex(500, 500);
	drawer.vertex(100, 500);
	drawer.color(0, 255, 0);
	drawer.vertex(500, 100);
	drawer.end();

	//drawer.begin(CIRCLES);
	//drawer.color(0, 255, 0);
	//drawer.vertex(300, 300);
	//drawer.color(0, 0, 0);
	//drawer.vertex(200, 250);
	//drawer.end();

	//draw_circle(drawer, glm::vec2(200, 300), 50, 5);
	//draw_box_curve(drawer, glm::vec2(300, 300), 200, 4.3, 0, 5000, 10);

	//drawer.color(0, 0, 255);
	//draw_parametric(drawer, [](float t) {

	//	float x = (11.0f * glm::cos(t) - 6.0f * glm::cos(11.0f / 6.0f * t)) * 10 + 300.0f;
	//	float y = (11.0f * glm::sin(t) - 6.0f * glm::sin(11.0f / 6.0f * t)) * 10 + 300.0f;
	//	return glm::vec2(x, y);
	//	}, 0, 100, 10);

	drawer.save("res/gradient_box.png");
	//Image c = drawer.get_canvas();
	//save_ppm(c, "res/test.ppm");

}

