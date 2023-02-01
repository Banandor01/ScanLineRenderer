#ifndef GAUGE_H
#define GAUGE_H

#include "widget.h"

class SpeedGauge : Widget
{
public:
    SpeedGauge(int16_t x, int16_t y, int16_t width, int16_t height,
                           int16_t min1, int16_t max1 );

    // Widget interface
    void Update(Message message, int16_t param1, int16_t param2);
    void Create ();

public:
    uint16_t backGround;

private:
   //void DrawNumber();
    void CreateArrow(bool draw);

    int value, oldValue;
    int aimValue, oldAimValue;

	int minvalue, maxvalue;

    uint16_t centerx, centery;
    char     radius1, radius2;

    Triangle * t1;
    Triangle * t2;

};

#endif // GAUGE_H
