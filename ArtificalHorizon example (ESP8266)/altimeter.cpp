#include "altimeter.h"

#ifdef ONPC
#include "../../ScaneLineRenderer/utils.h"
#else 
#include "../ScaneLineRenderer/scaneLineRenderer.h"
#endif


using namespace util;


void Altimeer::Create(ScaneLineRenderer & renderer)
{
    backColor1 = color565(10,0,80);
	numberBack = color565(5, 5, 5);

    renderer.AddDrawableObj(&center2Back);
    center2Back.SetValues(centerx,120,120,1, color565 (10,0,120));

    renderer.AddDrawableObj(&center1Back);
    center1Back.SetValues(centerx,120,95,1,backColor1);

    renderer.AddDrawableObj(&rt1);

    renderer.AddDrawableObj(&altBack1);
    renderer.AddDrawableObj(&altBack2);

    renderer.AddDrawableObj(&alt1);

    renderer.AddDrawableObj(&alt3);
    renderer.AddDrawableObj(&alt4);
    renderer.AddDrawableObj(&alt5);

    renderer.AddDrawableObj(&cover1);
    renderer.AddDrawableObj(&cover2);

    renderer.AddDrawableObj(&rt2);
    renderer.AddDrawableObj(&rt3);

    uint8_t o = 0;
    for (uint16_t i = 0; i<360; i+= 18) {

        Point vect (0,115);
        Rotate(vect, i);

        int v = 12; if (i %36 == 0) v = 0;

        Point vect2 (0, 98 - v);  Rotate(vect2, i);

        lines[o].SetLine(vect.x+centerx, vect.y+120, vect2.x+centerx, vect2.y+120,1, 0xffff);
        renderer.AddDrawableObj(&lines[o]);
        o++;
    }

    o = 0;
    for (int i=0; i < 40; i++) {   numberStrs[i] = 0; }
    for (int16_t i = 0; i < 10; i+=1) {
        itoa(o, &numberStrs[o*4], 10);
        Point vect (0, 95);
        Rotate(vect, - 180 + o*36);
        vect.x -= 3;
        vect.y -= 4;
        numbers[o].SetValue(vect.x + centerx, vect.y +120, 2,  &numberStrs[o*4], 1, 0xffff);
        renderer.AddDrawableObj(&numbers[o++]);
    }

    renderer.AddDrawableObj(&needle1);
    renderer.AddDrawableObj(&needle2);
    renderer.AddDrawableObj(&needle3);

    renderer.AddDrawableObj(&center);
    center.SetValues(centerx,120,8,1,0xffff);
}

void Altimeer::SetValues(ScaneLineRenderer &renderer, float pitch, float /*roll*/)
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

    // --- Numbers
    int x= centerx - 18 * 2.5;
    int y = 72;

    int tpitch = pitch;
    if (tpitch <0 ) tpitch = 0;

    unsigned char tenThousands = tpitch/10000;
    unsigned char thousands   = (tpitch % 10000)/ 1000;
    unsigned char hundreds     = (tpitch % 1000) / 100;

    const int16_t height = 7*3+3;
    const int16_t width  = 5*3+3;

    tentStr [0] = tenThousands + '0';  tentStr [1] =  thousands + '0'; tentStr[2] =0;
    alt1.SetValue(x, y , 3,  tentStr , 1, 0xffff);

    int prev = hundreds;
    if (prev < 0) prev = 9;

    itoa(hundreds *100, actualStr, 10);           //actual
    itoa((hundreds+1)%10 *100, nextStr, 10);      //next

    int16_t shift = (tpitch % 100) * height / 100;
    alt3.SetValue(x+ width*2, y - height + shift,3, nextStr, 1, 0xffff);   // next
    alt4.SetValue(x+ width*2, y +          shift,3, actualStr, 1, 0xffff); // actual

    if ((tpitch % 100) <= 50) {
         int prev = hundreds%10 -1;
         if (prev <0) prev = 9;
         itoa(prev *100, prevStr, 10);
         alt5.SetValue(x+ width*2, y + height + shift,3, prevStr, 1, 0xffff); // prev
    } else {

        itoa((hundreds+2)%10 *100, prevStr, 10);
        alt5.SetValue(x+ width*2, y - height*2 + shift,3, prevStr, 1, 0xffff); // prev
    }

    cover1.SetValues(x+36, y-36, 54, 24, 1, backColor1);
    cover2.SetValues(x+36, y+34, 54, 30, 1, backColor1);

	// number backgrunds
    altBack1.SetValues(x-2,y -3, 40,26,1, numberBack);
    altBack2.SetValues(x + 36,y-12, 53 , 66 ,1, numberBack);

	// frames
    rt1.SetValues(x-4,y -5, 44,30,2,1, 0xffff);
    rt2.SetValues(x+34,y - 13, 57 , 48,2,1, 0xffff);
    rt3.SetValues(x+34,y-3, 2, 26, 1,1, numberBack);

}

