#include "wButton.h"


//#include "..\ScaneLineLibraray\scanlinedtft.h"


/*Construct Button in x,y coordinate ***************************************************************************/
Button::Button (int x0, int y0, int w0, int h0, uint8_t id, char * text1) :
    Widget(x0, y0, w0, h0, id), text( text1 )
{      
    pressed = false;
    manualReset = false;
    handler = 0;    

}
/* Call in every loop on every Button **************************************************************************/
void Button::Update(Message message, int mx, int my) {
	bool oldstate = pressed;
	//Serial.print("Button: ");
	//Serial.println(ID);

	if (message == ClickDown) {
        if (mx > xo && mx < (xo + width) && my > yo && my < (yo + height)) pressed = !pressed;      // TODO
	}
	else if (message == ClickUp && !manualReset) {
        pressed = false;
    } else  {
        return;
    }

    if (oldstate != pressed) {										// Button changed
        UpdateDraw();		
		if (handler != 0) {	handler->CallBack1(pressed, ID);
			//Serial.print("Handler called! ");
			//Serial.println(pressed);
		}
        return;
    }
    //if (pressed && handler != 0)  handler->CallBack2(pressed, ID);     // continouse pressing
}
//****************************************************************************************************
void Button::SetState(bool newstate)
{
    if (newstate != pressed) {
       pressed = newstate;
       UpdateDraw();
    }
    pressed = newstate;
}
// ***************************************************************************************************
void Button::ReDraw ( ) {

    int x = xo; int y = yo;
    if (parentOrigo != 0) {
        x += parentOrigo->x;
        y += parentOrigo->y;
    }
	
	doRect  = sTFT->AddRectangle(x, y, width, height, 1, 0x5555);
	doRect1 = sTFT->AddAlphaBlendRectangle(x+1, y+1, width-2, height-2, 2, 0x0055);
    if (!text) return;
	uint16_t color = theme->BRIGHT;
	doText = sTFT->AddText(x+8, y + height / 2 - 4, 2, 1, text, color);
}

void Button::UpdateDraw()
{
	uint16_t color = theme->FB1; 
	if (pressed) color = theme->BRIGHT;
	doRect1->color = color;

	if (!text) return;
	color = theme->BRIGHT;
	if (pressed) color = theme->FB1;	
	doText->color = color;		
}
