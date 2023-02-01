#include "widget.h"

//*****************************************************************************************
Widget::Widget(int16_t x, int16_t y, int16_t w1, int16_t h1, int16_t id) :
    xo(x) , yo(y), width (w1), height(h1), ID (id)
{
    parentOrigo = 0;
}
//******************************************************************************************
void Widget::SetParentOrigo(Point *origo) {
    parentOrigo = origo;
}

void Widget::SetRenderer(ScaneLineRenderer *renderer1)
{
    Widget::renderer = renderer1;   
}

ScaneLineRenderer * Widget::renderer;

/*******************************************************************************************/

