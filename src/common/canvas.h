#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "Image.h"


struct Vertex {
    int x, y;
    Pixel color;
};

enum PrimitiveType {
    UNDEFINED_TYPE = 0, LINES, TRIANGLES, CIRCLES
};

class canvas
{
public:
    canvas(int w, int h);
    virtual ~canvas();

    // Save to file
    void save(const std::string& filename);

    // Draw primitives with a given type (either LINES or TRIANGLES)
    // For example, the following draws a red line followed by a green line
    // begin(LINES);
    //    color(255,0,0);
    //    vertex(0,0);
    //    vertex(100,0);
    //    color(0,255,0);
    //    vertex(0, 0);
    //    vertex(0,100);
    // end();
    void begin(PrimitiveType type);
    void end();

    // Specifiy a vertex at raster position (x,y)
    // x corresponds to the column; y to the row
    void vertex(int x, int y);

    // Specify a color. Color components are in range [0,255]
    void color(unsigned char r, unsigned char g, unsigned char b);

    // Fill the canvas with the given background color
    void background(unsigned char r, unsigned char g, unsigned char b);

    Image get_canvas() const { return m_Canvas; }

private:
    void draw_lines();
    void draw_line_low(Vertex v0, Vertex v1);
    void draw_line_high(Vertex v0, Vertex v1);
    void draw_triangles();
    void draw_circles();
private:
    Image m_Canvas;
    PrimitiveType m_DrawType = PrimitiveType::UNDEFINED_TYPE;
    Pixel m_Background = { 0, 0, 0 };
    Pixel m_Color = { 255, 255, 255 };
    std::vector<Vertex> m_Vertices;
};

#endif


