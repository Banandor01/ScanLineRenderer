
#include "wUpDownSpin.h"
#include "Window.h"

//#include <Arduino.h>

#ifndef ARDUINO
   // #include <stdio.h>
    #include <stdlib.h>
#endif


//*************************************************************************************************
UpDownSpinner::UpDownSpinner(int x0, int y0, uint8_t width1, uint8_t height, uint8_t maxValue1, uint8_t id) :
    Widget(x0, y0, width1, 0, id), // TOD
maxValue(maxValue1),
pressUp(false), pressDw(false),
opressedR(true), opressDw(true)
{
	value = 0;
	oldvalue = 10;
	size = height;
}
//*************************************************************************************************

void UpDownSpinner::Update(Message message, int mx, int my) {

	bool buttonPressed = false;
    if (message == ClickDown || message == Down) {
        int xv = xo + width;
		if (xv < mx && mx < (xv + size)) {
			if (yo < my && my < (yo + size / 2))
			{
				pressUp = true;
				buttonPressed = true;
				
			}
			else if ((yo + size / 2 + 2) < my && my < (yo + size)) {
				buttonPressed = true;
				pressDw = true;
			}
		}	
    } else if (message == ClickUp ) {
        pressUp = pressDw = false;
    }

    if (pressUp != opressedR || pressDw != opressDw || buttonPressed ) { 	// some buttons state changed
		updateValue();
        ReDrawButtons();
		opressedR = pressUp;
		opressDw = pressDw;				
		//if (handler != 0) handler->callBack1(pressed, ID);
		return;
	}
	//if (pressed && handler != 0)  handler->callBack2(pressed, ID);     // continouse pressing 
}
//*************************************************************************************************
void UpDownSpinner::SetValue(uint8_t v) {
	value = v;
	updateValue();
}
//*************************************************************************************************
void UpDownSpinner::ReDrawButtons() {
	int x, y;
	if (parentOrigo != NULL) {
		x = xo + parentOrigo->x;
		y = yo + parentOrigo->y;
	}
	else {
		x = xo;
		y = yo;
	}

    // Up Button
	int xv = x + width + 1;
  
	int color = theme->FB1;
    if (pressUp) color = theme->BRIGHT;
    int yv1 = y + size/2 -2;
    tft->fillTriangle(xv + size/2,  y + 3, xv + 8, yv1, xv + size-8, yv1, color);

    // Down Button
	
	color = theme->FB1;
    if (pressDw) color = theme->BRIGHT;
    int yv = y + size/2 +2 +2;
    yv1 = y + size -1;
    tft->fillTriangle(xv + 8,  yv, xv + size -8, yv, xv + size /2, yv1, color);
}

void UpDownSpinner::ReDraw() {
	oldvalue = value - 1;
	int x = xo;
	int y = yo;
	if (parentOrigo != NULL) {
		x += parentOrigo->x;
		y += parentOrigo->y;
	}

	int xv = x + width + 1;
	theme->drawBox(tft, xv, y + 1, size, size / 2, false);
	theme->drawBox(tft, xv, y + size / 2 + 2, size, size / 2, false);
	
	updateValue();
    ReDrawButtons();	
}

void UpDownSpinner::updateValue() {	
	if (pressDw && value > 0  )  value--;
	if (pressUp && value < maxValue)  value++; 

	int x = xo; 
	int y = yo;
	if (parentOrigo != NULL) {
        x = xo + parentOrigo->x;
        y = yo + parentOrigo->y;
	}

	if (oldvalue != value){
		tft->setTextColor(theme->BRIGHT, theme->BACK);
		theme->drawBox(tft,x, y, width, size+3, true);       
        char temp[5];
		itoa(value, temp, 10);		
        tft->drawCentreString(temp, x+width/2, y + size /2-8, 2);

		oldvalue = value;
	}
}



