#include "wprogress.h"


Progress::Progress(int x0, int y0, int w0, int h0, uint8_t id) :
    Widget(x0, y0, w0, h0, id)
{
    value = 40;
}

void Progress::Update(Message message, int param1, int param2)
{
    if (message == Down) {     
		value +=3;
		UpdateDraw();
    }
}

void Progress::ReDraw()
{
    int x = xo; 
	int y = yo;
    if (parentOrigo != 0) {
        x = xo + parentOrigo->x;
        y = yo + parentOrigo->y;
    }
	rect = sTFT->AddRectangle(x, y, width, height, 2, theme->BRIGHT);   
    int v = width * ( value +1 ) / 255 -2;
	inrect = sTFT->AddAlphaBlendRectangle(x + 1, y + 1, v, height-2, 1, theme->FB1);
}

void Progress::UpdateDraw()
{
	inrect->w = width * (value + 1) / 255 - 4;

}
