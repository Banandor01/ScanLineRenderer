#include "wtrack.h"
///#include "window.h"

//#define BIGWIDGET

TrackBar:: TrackBar (int x0, int y0, int w0, int h0, uint8_t id):
    Widget (x0, y0, w0, h0, id)
{
    value = oldvalue = 0;
}


#ifndef BIGWIDGET

void TrackBar::Update(Message message, int mx, int my)
{
	if (message == Down &&
		mx > xo && mx < (xo + width) && my > yo && my < (yo + height +10)) {
		oldvalue = value;
		value = mx - xo;
		if (value > width - 18) value = width - 18;
		UpdateDraw();
	}
}

void TrackBar::ReDraw()
{
	int x = xo;	int y = yo;

    if (parentOrigo != NULL) {
		x += parentOrigo->x;
        y += parentOrigo->y;
    }
    rect = sTFT->AddFilledRectangle1 (x      , y+5, width, height, 2, Widget::theme->FB1, theme->BRIGHT);
	rect1 = sTFT->AddFilledRectangle1 (x+value, y  , 15   ,  height + 10, 1, Widget::theme->FILL1, theme->BRIGHT);
}
void TrackBar::UpdateDraw()
{
	int x = xo;	int y = yo;
	if (parentOrigo != NULL) {
		x += parentOrigo->x;
		y += parentOrigo->y;
	}
	rect1->x = x + value;
}

#else

void TrackBar::Update(Message message, int mx, int my)
{

    if (message == Down) {
        if (mx > xo && mx < (xo + width -15) && my > yo && my < (yo + height)) {
            oldvalue = value;
            value = mx - xo;
        }

        ReDraw();
    }
}


void TrackBar::ReDraw()
{
    int x; int y;

    if (parentWindow != NULL) {
        x = xo + parentWindow->origo.x;
        y = yo + parentWindow->origo.y;
    } else {
        x = xo;
        y = yo;
    }
   // int v = width * ( value +1 ) / 255 -4;
   // int ov = width * ( value +1 ) / 255 -4;
    tft->fillRect(x+ oldvalue, y-5, 15, 25, theme->BACK);
    tft->drawRoundRect(x,y, width, 15, 4, theme->BRIGHT);
    //theme->drawBox(Widget::tft, x, y, width, 5, true);
    //theme->drawBox(Widget::tft, x+ value, y-5, 6, 15, false);


#endif
