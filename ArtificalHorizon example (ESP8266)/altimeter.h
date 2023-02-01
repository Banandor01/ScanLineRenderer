#ifndef ALTIMEER_H
#define ALTIMEER_H

#ifndef ONPC
#include "../ScaneLineRenderer/drawableObject.h"
#include "../ScaneLineRenderer/scaneLineRenderer.h"
#include <stdint.h>
#else
#include "../ScaneLineRenderer\drawableObject.h"
#include "../ScaneLineRenderer\scaneLineRenderer.h"
#endif


class Altimeer
{
public:
    Altimeer() { centerx = 142; }

    void Create(ScaneLineRenderer & renderer);
    void SetValues(ScaneLineRenderer & renderer, float pitch, float);

private:
    Triangle needle1, needle2, needle3;

    Line lines[20];
    Text numbers[10];
    char numberStrs[40];

    FilledCircle center;
    FilledCircle center2Back;
    FilledCircle center1Back;

    Text alt1, alt2, alt3, alt4, alt5;

    char tentStr[3];
    char nextStr[4];
    char prevStr[4];
    char actualStr[4];

    FilledRectangle altBack1;
    FilledRectangle altBack2;

    Rectangle rt1, rt2, rt3;

    FilledRectangle cover1;
    FilledRectangle cover2;

    int16_t centerx;
    uint16_t backColor1;
	uint16_t numberBack;
};

#endif // ALTIMEER_H
