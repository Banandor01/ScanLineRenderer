#include "drawableObject.h"
#include "glcdfont.c"

#define LINEWIDTH 320
#define TFTW 320

#ifndef ARDUINO
#include <math.h>
#endif

#include <Arduino.h>

#include "fixpoint.h"

#define swap(a, b, T)  { T t = a; a = b; b = t; }

#define ONPC

#ifdef ONPC
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

DrawableObj::DrawableObj(ObjectType type)
{
    m_objtype = type;
}

// -- Line ------------------------------------------------------------------------------------
Line::Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, unsigned char z, uint16_t color) : DrawableObj (LineT)
{
    if (y0 < y1) {
        this->oldx = x0;
        this->x0 = x0;
        this->y0 = y0;
        this->x1 = x1;
        this->y1 = y1;
    } else {
        this->oldx = x1;
        this->x0 = x1;
        this->y0 = y1;
        this->x1 = x0;
        this->y1 = y0;
    }
    this->m_color = color;
}

void Line::SetLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, unsigned char z, uint16_t color)
{
    if (y0 < y1) {
        this->oldx = x0;
        this->x0 = x0;
        this->y0 = y0;
        this->x1 = x1;
        this->y1 = y1;
    } else {
        this->oldx = x1;
        this->x0 = x1;
        this->y0 = y1;
        this->x1 = x0;
        this->y1 = y0;
    }
    this->m_color = color;
}

void Line::RenderLine(uint16_t actualLine, uint16_t * colorBuffer, uint16_t * /*deepBuffer */)
{
    if ( actualLine < y0 || actualLine > y1 ) return;

    fixedp dx = x1 - x0;
    fixedp dy = y1 - y0;

    int actualYStep = actualLine  - y0;
    // the first call
    if (actualYStep == 0) {
        oldx = x0;   
    }

    int xx = x0;
    if (dy != 0) {
        xx = x0 + dx/dy * actualYStep; 
    } else {
        xx = x0 + dx;
    }

    int tx = xx;

    if ( xx <= oldx ) {  swap (oldx, xx, int); }

    if (oldx > 319 || xx< 0) return;

    if (oldx < 0) oldx = 0;
    if (xx > 319) xx = 319;

    for ( ; oldx <= xx; oldx++ ){
        colorBuffer[oldx] = this->m_color;
    }

    oldx = tx;
}

// -- Triangle --------------------------------------------------------------------------------

Triangle::Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, char z, uint16_t color) : DrawableObj(TriangleT)
{
     // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1,int);
    }
    if (y1 > y2) {
        swap(y2, y1, int); swap(x2, x1,int);
    }
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1,int);
    }

    this->x0 =x0;    this->y0 =y0;
    this->x1 =x1;    this->y1 =y1;
    this->x2 =x2;    this->y2 =y2;

    this->color = color;
}


void Triangle::SetValue(Point p1, Point p2, Point p3, char z, uint16_t color)
{
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (p1.y > p2.y) { swap(p1, p2, Point); }
    if (p2.y > p3.y) { swap(p2, p3, Point); }
    if (p1.y > p2.y) { swap(p1, p2, Point); }

    this->x0 = p1.x;    this->y0 = p1.y;
    this->x1 = p2.x;    this->y1 = p2.y;
    this->x2 = p3.x;    this->y2 = p3.y;

    this->color = color;
}


void Triangle::SetValue(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                        char z, uint16_t color)
{
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1,int);
    }
    if (y1 > y2) {
        swap(y2, y1, int); swap(x2, x1, int);      
    }
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1, int);
    }

    this->x0 =x0;    this->y0 =y0;
    this->x1 =x1;    this->y1 =y1;
    this->x2 =x2;    this->y2 =y2;

    this->color = color;
}

void Triangle::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t *deepBuffer)
{
    if (actualLine < y0 || actualLine > y2) return;
    fixedp dx02 = x2 - x0,
           dy02 = y2 - y0;

      /* if(y1 == y2) last = y1;   // Include y1 scanline
        else         last = y1-1; // Skip it*/

    // triangle upper part
    if (actualLine >= y0 && actualLine < y1) {   
        int actualYStep = actualLine - y0;

        fixedp  dx01 = x1 - x0,
                dy01 = y1 - y0;

        int a = x0, b =x0;
        if (dy01 != 0) a = x0 +  dx01/dy01 * actualYStep;
        if (dy02 != 0) b = x0 +  dx02/dy02 * actualYStep;

        if (a>b) { swap(a,b, int); }

        if (b<0 || a> 319) return;
        if (a<0) a=0;
        if (b>319) b= 319;

        for ( ; a < b; a++) {
            colorBuffer[a] = color;
        }
    } else
    // triangle lower part
    {
        fixedp dx12 = x2 - x1,
               dy12 = y2 - y1;

        int a =x0, b = x1;
        int stepy02 =  actualLine - y0;
        int stepy12 =  actualLine - y1;

        if (dy02 != 0) a = x0 + dx02/dy02 * stepy02;
        if (dy12 != 0) b = x1 + dx12/dy12 * stepy12;

        if (a>b) { swap(a,b, int); }

        if (b<0 || a> 319) return;
        if (a<0) a=0;
        if (b>319) b= 319;

        for ( ; a < b; a++) {
            colorBuffer[a] = color;
        }

    }
}

TriangleTex::TriangleTex(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, char z, uint16_t color) : DrawableObj(TriangleT)
{
     // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1,int);
    }
    if (y1 > y2) {
        swap(y2, y1, int); swap(x2, x1,int);
    }
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1,int);
    }

    this->x0 =x0;    this->y0 =y0;
    this->x1 =x1;    this->y1 =y1;
    this->x2 =x2;    this->y2 =y2;

    this->color = color;
}

TriangleTex::TriangleTex(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                   Point t0, Point t1, Point t2, char z, uint16_t color) : DrawableObj(TriangleT)
{
    // Sort coordinates by Y order (y2 >= y1 >= y0)
   if (y0 > y1) {
       swap(y0, y1, int); swap(x0, x1, int);
       swap(t0,t1, Point);
   }
   if (y1 > y2) {
       swap(y2, y1, int); swap(x2, x1, int);
       swap(t2, t1, Point);
   }
   if (y0 > y1) {
       swap(y0, y1, int); swap(x0, x1, int);
       swap(t0, t1, Point);
   }

   this->x0 =x0;    this->y0 =y0;
   this->x1 =x1;    this->y1 =y1;
   this->x2 =x2;    this->y2 =y2;

   this->t1 = t1;
   this->t2 = t2;
   this->t0 = t0;

   this->color = color;
}

void TriangleTex::SetValue(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                        Point t0, Point t1, Point t2, char z, uint16_t color)
{
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1,int);
        swap(t0, t1, Point);
    }
    if (y1 > y2) {
        swap(y2, y1, int); swap(x2, x1, int);
        swap(t2, t1, Point);
    }
    if (y0 > y1) {
        swap(y0, y1, int); swap(x0, x1, int);
        swap(t0, t1, Point);
    }

    this->x0 =x0;    this->y0 =y0;
    this->x1 =x1;    this->y1 =y1;
    this->x2 =x2;    this->y2 =y2;

    this->t0 = t0;
    this->t1 = t1;
    this->t2 = t2;

}

void TriangleTex::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t *deepBuffer)
{
    if (actualLine < y0 || actualLine > y2) return;
    fixedp
            dx01 = x1 - x0,
            dy01 = y1 - y0,
            dx02 = x2 - x0,
            dy02 = y2 - y0;
    fixedp
            du02 = t2.x - t0.x,
            dv02 = t2.y - t0.y;

    /* if(y1 == y2) last = y1;   // Include y1 scanline
        else         last = y1-1; // Skip it*/

    // triangle upper part
    if (actualLine >= y0 && actualLine < y1) {
        int actualYStep = actualLine - y0;

        fixedp du01 = t1.x - t0.x,
               dv01 = t1.y - t0.y;

        int a = x0, b =x0;
        if (dy01 != 0) a = x0 +  dx01/dy01 * actualYStep;
        if (dy02 != 0) b = x0 +  dx02/dy02 * actualYStep;

        int au = t0.x + (du01 /dy01 * actualYStep).GetInt();
        int av = t0.y + (dv01 /dy01 * actualYStep).GetInt();
        int bu = t0.x + (du02 /dy02 * actualYStep).GetInt();
        int bv = t0.y + (dv02 /dy02 * actualYStep).GetInt();

        if (a>b) {
            swap(a,b, int);
            swap(au, bu, int);
            swap(av, bv, int);
        }

        int steps = b-a;
        if (steps ==0) steps =1;
        fixedp ustep = fixedp (bu - au) / steps;
        fixedp vstep = fixedp (bv - av) / steps;

        fixedp us = au;
        fixedp vs = av;

        for ( ; a < b; a++) {
            unsigned int c = Texture(us.GetInt(),vs.GetInt());

            colorBuffer[a] = c;
            us = us + ustep;
            vs = vs + vstep;
        }
    } else
    // triangle lower part
    {
        fixedp dx12 = x2 - x1,
               dy12 = y2 - y1;

        fixedp
               du12 = t2.x - t1.x,
               dv12 = t2.y - t1.y;

        int a =x0, b = x1;
        int stepy02 =  actualLine - y0;
        int stepy12 =  actualLine - y1;

        if (dy02 != 0) a = x0 + dx02/dy02 * stepy02;
        if (dy12 != 0) b = x1 + dx12/dy12 * stepy12;

        int au = t0.x + (du02 /dy02 * stepy02).GetInt();
        int av = t0.y + (dv02 /dy02 * stepy02).GetInt();

        int bu = t1.x;
        int bv = t1.y;
        if (dy12 != 0) {
            bu = t1.x + (du12 /dy12 * stepy12).GetInt();
            bv = t1.y + (dv12 /dy12 * stepy12).GetInt();
        }


        if (a>b) {
            swap(a,b, int);
            swap(au, bu, int);
            swap(av, bv, int);
        }


        int steps = abs(b-a);
        if (steps ==0) steps =1;
        fixedp ustep = fixedp (bu - au) / steps;
        fixedp vstep = fixedp (bv - av) / steps;

        fixedp us = au;
        fixedp vs = av;

        for ( ; a < b; a++) {
            unsigned int c = Texture(us.GetInt(), vs.GetInt());

            colorBuffer[a] = c; // | 0xf000;
            us = us + ustep;
            vs = vs + vstep;
        }

    }
}

// -- Rectangle --------------------------------------------------------------------------------
Rectangle::Rectangle(int16_t x1, int16_t y1, int16_t w1, int16_t h1,
                     unsigned char s, unsigned char z, int16_t color) : DrawableObj(RectangleT)
{
    this->x =  x1 >0 ? x1 : 0;
    this->y = y1;
    this->w = x1 >0 ? w1 : w1 + x1;
    this->h = h1;
    this->color = color;
    this->s = s;
}

void Rectangle::SetValues(int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char s, unsigned char z, int16_t color)
{
    this->x =  x1 >0 ? x1 : 0;
    this->y = y1;
    this->w = x1 >0 ? w1 : w1 + x1;
    this->h = h1;
    this->color = color;
    this->s = s;
}

void Rectangle::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t *deepBuffer)
{
    if ( actualLine < y || actualLine > y+h) return;

    int maxx =  x+w ;
    if (maxx > LINEWIDTH) maxx = LINEWIDTH;


    if (s == 2) {
         if (y+1 == actualLine || y + h-1 == actualLine){
            for (int xx =x; xx < maxx; xx++) {
                colorBuffer[xx] = color;
             }
            return;
         }
    }

    if (y == actualLine || y + h == actualLine){
        for (int xx =x; xx < maxx; xx++) {
            colorBuffer[xx] = color;
        }
    } else {
        colorBuffer[x] = color;
        maxx--;
        colorBuffer[maxx] = color;
        if (s == 2 ) {
            colorBuffer[x+1] = color;
            maxx--;
            colorBuffer[maxx] = color;
        }
    }
}

// -- FilledRectangle ----------------------------------------------------------------------------------
FilledRectangle::FilledRectangle(int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char z, uint16_t color) :
    DrawableObj (FilledRectangleT)
{
    this->x =  x1 >0 ? x1 : 0;
    this->y = y1;
    this->w = x1 >0 ? w1 : w1 + x1;
    this->h = h1;
    this->color = color;
    this->z = z;
}

void FilledRectangle::SetValues(int16_t x1, int16_t y1, int16_t w1, int16_t h1, unsigned char z, uint16_t color)
{
    this->x =  x1 >0 ? x1 : 0;
    this->y = y1;
    this->w = x1 >0 ? w1 : w1 + x1;
    this->h = h1;
    this->color = color;
    this->z = z;
}

void FilledRectangle::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t * /*deepBuffer*/)
{
    if ( actualLine < y || actualLine > y+h) return;

    int maxx =  x+w ;
    if (maxx > LINEWIDTH) maxx = LINEWIDTH;
    for (int xx = x; xx < maxx; xx++) {
        colorBuffer[xx] = color;
    }
}

// -------------------------------------------------------------------------------------------------------------
FilledRectWithFrame::FilledRectWithFrame(int16_t x1, int16_t y1, int16_t w1, int16_t h1, char z, uint16_t color, uint16_t color2, bool transparent) :
    DrawableObj(FilledRectangleT1)
{
    this->x =  x1 >0 ? x1 : 0;
    this->y = y1;
    this->w = x1 >0 ? w1 : w1 + x1;
    this->h = h1;
    this->color = color;
    this->color2 = color2;
	this->transparent = transparent;
}

void FilledRectWithFrame::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t *deepBuffer)
{
    if (actualLine < y || actualLine > y + h) return;
    int maxx = x + w;
    if (maxx > LINEWIDTH) maxx = LINEWIDTH;

    if (y == actualLine || y + h == actualLine)
    {
        for (int xx = x; xx < maxx; xx++) {
            colorBuffer[xx] = color2;
        }
    }
    else {
        maxx--;
		if (transparent) {
			for (int xx = x + 1; xx < maxx; xx++) {
				colorBuffer[xx] |= color;
			}
		}
		else {
			for (int xx = x + 1; xx < maxx; xx++) {
				colorBuffer[xx] = color;
			}
		}
		
        colorBuffer[x] = color2;
        colorBuffer[maxx] = color2;
    }
}

// -------------------------------------------------------------------------------------------------------------
Circle::Circle(int16_t x, int16_t y, int16_t r, char z, uint16_t color) : DrawableObj (CircleT)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->color = color;
    this->z= z;
}

void Circle::SetValues(int16_t x, int16_t y, int16_t r, char z, uint16_t color)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->color = color;
    this->z= z;
}

void Circle::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t * /*deepBuffer */)
{
    if ( actualLine < y- r || actualLine > y+r) return;

    int16_t r2 = r * r;
    int16_t hy = y - actualLine;
    int16_t hy1;

    if ( actualLine == y - r) {
        hy1 = (hy) * (hy);
    } else {
        hy1 = (hy-1) * (hy-1);
    }

    hy = hy * hy;

    int16_t dx0 =  (int16_t) sqrt(double(r2-hy1));
    int16_t dx1 =  (int16_t) sqrt(double(r2-hy));

    int16_t x01 = x - dx0;
    int16_t x02 = x + dx0;

    int16_t x1 = x - dx1;
    int16_t x2 = x + dx1;

    if (x1 <= x01) {
       for (; x1 <= x01; x1++) {
            colorBuffer[x1] = color;
        }
       for (; x02 <= x2; x02++) {
               colorBuffer[x02] = color;
       }
    } else {
        for (; x01 <= x1; x01++) {
             colorBuffer[x01] = color;
         }
        for (; x2 <= x02; x2++) {
                colorBuffer[x2] = color;
        }
    }
}

// --- FilledCircle --------------------------------------------------------------------------------------------
FilledCircle::FilledCircle(int16_t x, int16_t y, int16_t r, char z, uint16_t color) : DrawableObj (CircleT)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->color = color;
    this->z= z;
}

void FilledCircle::SetValues(int16_t x, int16_t y, int16_t r, char z, uint16_t color)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->color = color;
    this->z= z;
}

void FilledCircle::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t * /*deepBuffer */)
{
    if ( actualLine < y- r || actualLine > y+r) return;

    int16_t r2 = r * r;
    int16_t hy = y - actualLine;
    hy = hy * hy;

    int16_t dx =  (int16_t) sqrt(double(r2-hy));

    int16_t x1 = x - dx;
    int16_t x2 = x + dx;

    for (; x1 < x2; x1++) {
        colorBuffer[x1] = color;
    }
}

// --- Text --------------------------------------------------------------------------------------------
Bitmap::Bitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t * data, unsigned char z) : DrawableObj(ObjectType::TextT) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->data = data;

}

void Bitmap::SetValue(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t * data, unsigned char z) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->data = data;
}

void Bitmap::RenderLine(uint16_t actualLine, uint16_t * colorBuffer, uint16_t * deepBuffer) {

	if (actualLine < y || actualLine > y + h) return;
	if (data == NULL) return;

	int16_t rightside = (x + w) < 320 ? (x + w) : TFTW;

    uint16_t * p = data + (actualLine-y) * w;
	for (int i = x; i < rightside; i++) {
        //if (*p == 0) continue;
		colorBuffer[i] = *p;
		p++;
	}

}

// --- Text --------------------------------------------------------------------------------------------

void Text::SetValue(int16_t x, int16_t y, uint8_t size, char * text, unsigned char z, uint16_t color)
{
	this->x = x;
	this->y = y;
	this->size = size;
    this->text = text;

}

void Text::DrawCharLine(int16_t sx, int16_t line, uint16_t * colorBuffer, char cha)
{
    for (int8_t j = 0; j < 5*size; j++) {
         uint8_t c = pgm_read_byte(font + (cha * 5) + j/size);
         if (c & (0x1 << line)) {
            colorBuffer[sx + j] = 0xffff;
         }
    }
}


void Text::RenderLine(uint16_t actualLine, uint16_t * colorBuffer, uint16_t */* deepBuffer*/)
{
    if (actualLine < y || actualLine > (y + 7 * size)) return;
    if (text == NULL) return;
    int sx = x;
    int16_t line = (actualLine - y) / size;

    int i = 0;
    char* ptext = text;
    while (*ptext != NULL) {
        DrawCharLine(sx, line, colorBuffer, *ptext++);
        sx += 6 * size ;
	}
}

// Rotated Text ------------------------------------------------------------------------------

namespace  {

const float DEGTORAD = 3.14159f / 180.0f;
void Rotate(Point & p, float angle) {
    // angle += 720;

    float cosValue = cos(angle * DEGTORAD);
    float sinValue = sin(angle * DEGTORAD);

    int nx = cosValue * p.x - sinValue *p.y;
    int ny = sinValue * p.x + cosValue *p.y;

    p.x = nx;
    p.y = ny;
}

void Rotate(Point & p, Point & vector) {
    int nx = vector.x * p.x - vector.y *p.y;
    int ny =  p.x * vector.y + p.y * vector.x;
    // x'=x\cos \theta -y\sin \theta \,,
    // y'=x\sin \theta +y\cos \theta \,
    p.x = nx / 100;
    p.y = ny / 100;
}

} // end of anonimus namespace

// Rotated Text ------------------------------------------------------------------------
void RotatedText::SetValue(uint16_t x, uint16_t y, float angle, char *text)
{
    this->angle = angle;
    this->text = text;

    length = 0;
    char * p =text;
    while (*p != NULL) {
        p++;
        length ++;
    }

    Point rotVector (100,0);
    Rotate(rotVector, angle);
    int16_t w = length*5;
    int16_t h = 7;

    Point points[4] = {
        { -w,-h } ,
        {  w,-h } ,
        {  w,h } ,
        { -w,h }
    };
    Rotate (points[0], rotVector);
    Rotate (points[1], rotVector);
    Rotate (points[2], rotVector);
    Rotate (points[3], rotVector);

    int16_t minx, maxx, miny, maxy;
    minx  = miny = 100;
    maxx  = maxy = -100;

    for (uint8_t i = 0; i < 4; i++) {
        if (minx > points[i].x) minx = points[i].x;
        if (miny > points[i].y) miny = points[i].y;
        if (maxx < points[i].x) maxx = points[i].x;
        if (maxy < points[i].y) maxy = points[i].y;
    }

    this->x = x;
    this->y = y;

    this->minx = minx;
    this->miny = miny;
    this->maxx = maxx;
    this->maxy = maxy;
}

uint16_t GetTextPixel (Point & point, char * text) {
    uint8_t letterN = point.x /5;
    uint8_t x = point.x %5;

    uint8_t line = point.y %7;
    char cha = text[letterN];

         uint8_t c = pgm_read_byte(font + (cha * 5) + x);
         if (c & (0x1 << line)) {
            return  0xffff;
         }
    return 0;
}


bool  RotatedText::Calcualte (int16_t x, int16_t y, Point & vector, Point & textPoint) {
    // it could be optimized, as a simple vector addition
    int xx = (x * vector.x - y *vector.y) / 100;
    int yy = (x * vector.y + y * vector.x) / 100;

    int w = length*5;

    xx+= w;
    yy+=7;

    w*= 2;
    // the screen coordinates within in the Texture ?
    if (xx >= 0 && xx < w && yy >=0 && yy <14) {
        textPoint.x = xx/2;
        textPoint.y = yy/2;
        return true;
    }
    return false;
}

void RotatedText::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t *deepBuffer)
{
    if (actualLine < y+miny || actualLine > y+maxy) return;

    actualLine = actualLine - y;

    Point vector(100,0);
    Rotate (vector, - angle);

    Point textPoint;
    int o = 0;
    for (int i = minx; i < maxx; i ++) {
        if ( Calcualte(i, actualLine,vector, textPoint) ) {
            uint16_t c = GetTextPixel(textPoint, (char* ) text);
            if (c != 00)
            colorBuffer [this->x+i] = c;
        } else {
           // colorBuffer [this->x+i] = 0xff00;
        }
        o++;
    }
}

// Slice -------------------------------------------------------------------------------------------------------

void Slice::SetValue(int16_t x, int16_t y, int16_t r, int16_t start, int16_t stop, uint8_t z, uint16_t color)
{
    this->origo.x = x;
    this->origo.y = y;
    this->r = r;


    this->start = start < 0 ? start +360 : start %360;
    this->stop =stop < 0 ? stop +360 : stop %360;
    this->z =z;
    this->color = color;

    
}

void Slice::RenderLine(uint16_t actualLine, uint16_t *colorBuffer, uint16_t *deepBuffer)
{
    if ( actualLine < origo.y- r || actualLine > origo.y+r) return;

    int16_t r2 = r * r;
    int16_t hy = ( origo.y -actualLine );

    int16_t xCut=  (int16_t) sqrt(double(r2-hy*hy));

    int16_t angle1 = atan2(hy, xCut) * 180.0f / 3.14159f;
    int16_t angle2 = 90 + (90-angle1);

    if(angle1 < 0) angle1 += 360;


    // TODO
    if (hy == r) { oldXcut = xCut; }

    int16_t temp = xCut;

    if (oldXcut > xCut) swap(oldXcut, xCut, int);

    if (start < stop ) {

        if ( angle1 < stop && angle1 > start) {
            int16_t step = oldXcut;
            for ( ; step <= xCut; step++) {
                colorBuffer[origo.x + step] =  color;
            }
        }

        if (angle2 < stop && angle2> start){
            int16_t step = oldXcut;
            for ( ; step <= xCut; step++) {
                colorBuffer[origo.x - step] =  color;
            }
        }
    } else {
        if ( angle1 > start || angle1 < stop) {
            int16_t step = oldXcut;
            for ( ; step <= xCut; step++) {
                colorBuffer[origo.x + step] =  color;
            }
        }

        if ( angle2 > start || angle2< stop) {
            int16_t step = oldXcut;
            for ( ; step <= xCut; step++) {
                colorBuffer[origo.x - step] =  color;
            }
        }
    }
    oldXcut = temp;
}
