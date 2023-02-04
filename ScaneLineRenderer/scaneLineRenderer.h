#ifndef SCANLINEDTFT_H
#define SCANLINEDTFT_H

#ifndef ONPC
#include "Arduino.h"
#endif // !ONPC

#include "drawableObject.h"
#include "objecttypes.h"

class ScaneLineRenderer
{
public:
    ScaneLineRenderer(unsigned int width);

    void ResetObjects() { objectNumber = 0; }

    void AddDrawableObj(DrawableObj* obj);
    void RemoveDrawableObj(DrawableObj* objectToRemove);

    void RenderObjectsInLine(unsigned int actualLine);

    uint16_t * GetColorBuffer() {
        return lineColors;
    }
    uint16_t* GetZBuffer() {
        return lineColors;
    }

private:
	uint16_t lineColors[320];
    uint16_t lineDeeps[320];

    DrawableObj* objects[100];
    uint16_t objectNumber;
    int16_t  tftWidth;
};

#endif // SCANLINEDTFT_H
