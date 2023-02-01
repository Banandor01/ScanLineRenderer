#ifndef AIRSPEEDINDICATOR_H
#define AIRSPEEDINDICATOR_H

#ifndef ONPC
#include "../ScaneLineRenderer/drawableObject.h"
#include "../ScaneLineRenderer/scaneLineRenderer.h"
#include <stdint.h>
#else
#include "../ScaneLineRenderer\drawableObject.h"
#include "../ScaneLineRenderer\scaneLineRenderer.h"
#endif

class AirSpeedIndicator
{    
public:
    AirSpeedIndicator() {
        centerx = 176;
        centery = 120;
    }

public:
    Slice sliceGreen[8];
    Slice sliceWhite[8];
    Slice sliceYellow[8];
    Line lines[40];
    Text texts[40];
    char strs[40][5];

    Triangle needle1;
    Triangle needle2;
    Triangle needle3;

    void Create (ScaneLineRenderer & renderer);
    void SetValues(ScaneLineRenderer &renderer, float pitch, float /*roll*/);

    int16_t centerx, centery;
};

#endif // AIRSPEEDINDICATOR_H
