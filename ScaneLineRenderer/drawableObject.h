#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "objecttypes.h"
#include <stdint.h>

//----------------------------------------------------------------------------------------------------
class Point {
public:
    Point (int16_t x, int16_t y) {
        this->x = x;
        this->y = y;
    }
    Point () {x = y = 0; }
    int16_t x, y;
};

//----------------------------------------------------------------------------------------------------
class DrawableObj {
public:
    DrawableObj (ObjectType type);
    virtual void RenderLine (uint16_t line, uint16_t * ColorBuffer, uint16_t * DeepBuffer) = 0;
    ObjectType GetType () { return m_objtype;}

protected:
    ObjectType m_objtype;
};

// ---------------------------------------------------------------------------------------------------
class Line : public DrawableObj {
public:
    Line() : DrawableObj(LineT),
        x0(0), y0(0), x1(0), y1(0), oldx(0), m_color(0) 
    {}
    Line (int16_t x0, int16_t y0, int16_t x1, int16_t y1, unsigned char z, uint16_t color);
    void SetLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, unsigned char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x0,y0, x1, y1;
    int16_t oldx;
    uint16_t m_color;
};

// ---------------------------------------------------------------------------------------------------
class Triangle : public DrawableObj {
public:
    Triangle () : DrawableObj(TriangleT) {}
    Triangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
    void SetValue(Point p1, Point p2, Point p3, char z, uint16_t color);
    void SetValue(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,  char z, uint16_t color);

//private:
    int16_t x0, y0, x1, y1, x2, y2;
    char z;
    uint16_t color;
};
// ---------------------------------------------------------------------------------------------------
/*class TriangleTex : public DrawableObj {
public:
    TriangleTex (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, char z, uint16_t color);
    TriangleTex (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
              Point t1, Point t2, Point t3,     char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
    void SetValue(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                  Point t1, Point t2, Point t3,     char z, uint16_t color);

//private:
    int16_t x0, y0, x1, y1, x2, y2;
    Point t0, t1, t2;
    char z;
    uint16_t color;
};*/

// ---------------------------------------------------------------------------------------------------
class FilledTriangle : public DrawableObj {
public:
    FilledTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t z);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
//private:
    int16_t x0, y0, x1, y1, x2, y2;
    uint16_t color;
};

// ---------------------------------------------------------------------------------------------------
class Rectangle : public DrawableObj {
public:
    Rectangle () : DrawableObj (RectangleT) {}
    Rectangle(int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char s, unsigned char z, int16_t color);
    void SetValues (int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char s, unsigned char, int16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x,y,w,h,z;
    uint16_t color;
    unsigned char s;
};

// ---------------------------------------------------------------------------------------------------
class FilledRectangle : public DrawableObj {
public:
    FilledRectangle () : DrawableObj(FilledRectangleT) {}
    FilledRectangle(int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char z, uint16_t color);
    void SetValues (int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x,y,w,h;
    uint16_t color;
    unsigned char z;
};

// ---------------------------------------------------------------------------------------------------
class FilledRectWithFrame : public DrawableObj {
public:
    FilledRectWithFrame () : DrawableObj (FilledRectangleT1) {}
    FilledRectWithFrame (int16_t x1, int16_t y1, int16_t w1, int16_t h1, char z, uint16_t color, uint16_t color2, bool transparent);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
	bool transparent;
    int16_t x,y,w,h;
    char z;
    uint16_t color, color2;
};

// ---------------------------------------------------------------------------------------------------
/*class RectangleCut : public DrawableObj {
public:
    RectangleCut (int16_t x1, int16_t y1, int16_t w1, int16_t h1, char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x,y,w,h;
    char z;
    uint16_t color;
};*/

// ---------------------------------------------------------------------------------------------------
class Circle :  public DrawableObj {
public:
    Circle() : DrawableObj(CircleT){};
    Circle (int16_t x, int16_t y, int16_t r, char z, uint16_t color);
    void SetValues (int16_t x, int16_t y, int16_t r, char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x,y,r;
    uint16_t color;
    char z;
};

// ---------------------------------------------------------------------------------------------------
class FilledCircle : public DrawableObj {
public:
    FilledCircle() : DrawableObj(CircleT){};
    FilledCircle (int16_t x, int16_t y, int16_t r, char z, uint16_t color);
    void SetValues (int16_t x, int16_t y, int16_t r, char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x,y,r;
    char z;
    uint16_t color;
};

// ---------------------------------------------------------------------------------------------------
class Hole : public DrawableObj {
public:
    Hole (int16_t x, int16_t y, int16_t r, char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
private:
    int16_t x,y,r;
    uint16_t color;
    char z;
};

// ---------------------------------------------------------------------------------------------------
class Text : public DrawableObj {
public:
    Text(): DrawableObj (TextT) { text = 0; }
    void SetValue(int16_t x, int16_t y, uint8_t size, char * text, unsigned char z, uint16_t color);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t *);
private:
    void DrawCharLine(int16_t sx, int16_t line, uint16_t * colorBuffer, char cha);

    int16_t x, y;

    unsigned char z;
    uint16_t color;
	char * text;
    uint8_t size;
};

// ---------------------------------------------------------------------------------------------------
class RotatedText : public DrawableObj {
public:
    RotatedText() : DrawableObj (ObjectType::TextT) {}
    void SetValue (uint16_t x, uint16_t y, float angle, char * text);
    void RenderLine (uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
    bool Calcualte (int16_t x, int16_t y, Point & vector, Point & textPoint);
private:
    int16_t x, y, size;
    unsigned char z;
    uint16_t color;
    char * text;
    uint8_t length;
    int16_t minx, miny;
    uint8_t maxx,maxy;
    float angle;
};

// ---------------------------------------------------------------------------------------------------
class Bitmap : public DrawableObj {
public:
	Bitmap() : DrawableObj(TextT) { data = 0; }
	Bitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t * data, unsigned char z);
	void SetValue(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t * data, unsigned char z);
	void RenderLine(uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);
	int16_t x, y;
	uint8_t w, h;
	uint16_t * data;   
};

class Slice : public DrawableObj {
public:
    Slice () : DrawableObj(CircleT) {}
    void SetValue (int16_t x, int16_t y, int16_t r, int16_t start, int16_t stop, uint8_t z,
                   uint16_t color);

    void RenderLine(uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer);

    Point origo;
    int16_t r, start, stop;
    uint8_t z;
    uint16_t color;
    int16_t oldXcut;
    
    int16_t startLeft, stopLeft;
    int16_t startRight, stopRight;
};

#endif // DRAWABLEOBJECT_H


