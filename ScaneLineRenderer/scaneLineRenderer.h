#ifndef SCANLINEDTFT_H
#define SCANLINEDTFT_H

#include "Arduino.h"

#include "drawableObject.h"
#include "objecttypes.h"

class ScaneLineRenderer
{
public:
    ScaneLineRenderer(int width);

    void ResetObjects() { objectNumber = 0; }
	void ResetObjects2() { objectNumber2 = 0; }
    void AddDrawableObj (DrawableObj * obj);

    void RenderObjectsInLine(unsigned int actualLine);

    uint16_t LineColors[320];
    uint16_t LineDeeps[320];

private:
    uint16_t objectNumber, objectNumber2;
    int16_t  tftWidth;
    DrawableObj * objects2[100];  
};

#endif // SCANLINEDTFT_H
