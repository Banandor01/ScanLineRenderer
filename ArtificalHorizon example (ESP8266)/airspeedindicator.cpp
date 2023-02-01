#include "airspeedindicator.h"

#ifdef ONPC
#include "../ScaneLineRenderer/utils.h"
#else
#include "../ScaneLineRenderer/utils.h"
#endif

using namespace util;

void AirSpeedIndicator::Create(ScaneLineRenderer &renderer)
{
    for (int i = 0; i < 5; i++) {
        renderer.AddDrawableObj(&sliceGreen[i]);
        renderer.AddDrawableObj(&sliceWhite[i]);
        renderer.AddDrawableObj(&sliceYellow[i]);
    }

    for (int i =0; i < 5; i++) {
        sliceGreen[i].SetValue(centerx,120,115+i,  200, 20, 1, color565(0,255,0));
        sliceWhite[i].SetValue(centerx,120,110+i,  240, 50, 1, color565(240,240,240));
        sliceYellow[i].SetValue(centerx,120,115+i, 130, 200,1, color565(255,255,0));
    }

    // Lines
    for (uint16_t i = 140, o = 0; i< 400; i+= 10) {

        Point vect (119,0);
        Rotate(vect, i);

        Point vect2 (100,0);
        Rotate(vect2, i);

        int v = 12;
        if (i %20 == 0) v = 0;

        lines[o].SetLine(vect.x+centerx, 120 -vect.y, vect2.x+centerx, 120 -vect2.y,1, 0xffff);
        renderer.AddDrawableObj(&lines[o]);
        o++;
    }

    // Numbers
    for (int16_t i = 50, o = 4; i > - 220; i-= 20) {

        Point vect (85,0);
        Rotate(vect, i);

        itoa(o*10, strs[o], 10);
        int16_t shift = 8;
        if (o*10 >= 100) shift = 12;

        texts[o].SetValue(vect.x + centerx-shift, 120 -vect.y-7, 2, strs[o], 1, 0xffff);
        renderer.AddDrawableObj(&texts[o]);
        o+=2;
    }

    // Needle
    renderer.AddDrawableObj(&needle1);
    renderer.AddDrawableObj(&needle2);
    renderer.AddDrawableObj(&needle3);
}

void  AirSpeedIndicator::SetValues(ScaneLineRenderer &renderer, float pitch, float /*roll*/)
{
    // needle
    Point rotateV (100,0);
    Rotate(rotateV, -180 + (((int)pitch)%1000)*360/1000 );  // TODO Clean

    Point p1(3, 80);	Point p3(0, 100);
    Point p2(-3, 80);

    Rotate(p1, rotateV);    Rotate(p2, rotateV);
    Rotate(p3, rotateV);

    p1.x += centerx;    p2.x += centerx;    p3.x += centerx;
    p1.y += 120;    p2.y += 120;    p3.y += 120;

    needle1.SetValue(p1, p2, p3,1, 0xeeee);

    p3 = Point ( -5,-20);
    Point p4   ( 5,-20);

    Rotate(p3, rotateV);
    Rotate(p4, rotateV);

    p3.x += centerx;    p4.x += centerx;
    p3.y += 120;    p4.y += 120;

    needle2.SetValue(p1, p2, p3,1, 0xeeee);
    needle3.SetValue(p4, p3, p1,1, 0xeeee);
}
