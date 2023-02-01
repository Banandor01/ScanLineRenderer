#include "wgauge.h"
//#include <Arduino.h>

//#ifndef ARDUINO
    #include <math.h>
//#endif



#define DEGTORAD 0.01745329251f

SpeedGauge::SpeedGauge(int16_t x, int16_t y, int16_t width, int16_t height,
                       int16_t min1, int16_t max1 ) :
    Widget(x, y, width, height,0),
    maxvalue(max1), minvalue(min1)
{
	value = minvalue;
    oldValue = value;

    centerx = x + width/2;
    centery = y + height * 1.3f;

    radius1 = height * 1.25f;
    radius2 = radius1 * 0.9f;

}

void SpeedGauge::Update(Message message, int16_t param1, int16_t param2)
{
    value  = param1;
    aimValue = param2;

    CreateArrow(false);

    oldAimValue = aimValue;
    oldValue = value;
}



// Draw Speed Gauge
void SpeedGauge::Create()
{
    float xd;
    float yd;

    int16_t p0x, p1x, p2x, p3x;
    int16_t p0y, p1y, p2y, p3y;

    int16_t colorr = 255;
    int16_t colorg;

#define MINDEG1 60
#define MAXDEG1 120

    for (int16_t it = MAXDEG1; it >= MINDEG1; it -= 5)
    {

        xd = cos(float(it) *  DEGTORAD);
        yd = sin(float(it) *  DEGTORAD);

        p0x = centerx + xd * radius1;
        p0y = centery - yd * radius1;

        p1x = centerx + xd * radius2;
        p1y = centery - yd * radius2;

        xd = cos(float(it + 2) * DEGTORAD);
        yd = sin(float(it + 2) * DEGTORAD);

        p2x = centerx + xd * radius1;
        p2y = centery - yd * radius1;

        p3x = centerx + xd * radius2;
        p3y = centery - yd * radius2;

        colorr = 255 - (it + 45) / 1.2;
        colorg = (it + 45) / 1.2;

        renderer->AddDrawableObj(new Triangle(p0x, p0y, p1x, p1y, p2x, p2y, 1, 0xff00));
        renderer->AddDrawableObj(new Triangle(p2x, p2y, p3x, p3y, p1x, p1y, 1, 0xee00));
    }

   // DrawNumber();
    CreateArrow(true);

}


namespace {
    int16_t map (int16_t value, float inMin , float inMax,  float outMin, float outMax) {

        if (value <= inMin) return outMin;
        if (value >= inMax) return outMax;

        return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }
}


void SpeedGauge::CreateArrow(bool draw) {

    uint16_t p1x, p1y, p2x, p2y, p0x, p0y, p3x, p3y;

    int16_t deg1;
    if (draw) {
        deg1 = map (value, minvalue, maxvalue, MAXDEG1, MINDEG1);
    } else {
        deg1 = map (oldValue, minvalue, maxvalue, MAXDEG1, MINDEG1);
    }

    float deg = (deg1 -1) *  DEGTORAD;
    p0x = centerx + cos(deg) * (radius2 - 3);
    p0y = centery - sin(deg) * (radius2 - 3);

    deg = (deg1 +1) *  DEGTORAD;
    p1x = centerx + cos(deg) * (radius2 - 3);
    p1y = centery - sin(deg) * (radius2 - 3);

    deg = (deg1 +2) *  DEGTORAD;
    p3x = centerx + cos(deg) * (50);
    p3y = centery - sin(deg) * (50);

    deg = (deg1 -2) *  DEGTORAD;
    p2x = centerx + cos(deg) * (50);
    p2y = centery - sin(deg) * (50);

    if (draw) {
        t1 = new Triangle(p1x, p1y, p0x, p0y, p2x, p2y, 1, 0x0ff0);
        t2 = new Triangle(p1x, p1y, p2x, p2y, p3x, p3y, 1, 0x0ff0);
        renderer->AddDrawableObj(t1);
        renderer->AddDrawableObj(t2);

    } else {
     //   t1->SetValue(p1x, p1y, p0x, p0y, p2x, p2y, 1);
      //  t2->SetValue(p1x, p1y, p2x, p2y, p3x, p3y, 1);
    }
}

/*
void Gauge::DrawTempNumber() {
    tft->fillCircle(centerx, centery, radius2 - 25, tft->color565(30, 30, 120));
    uint16_t8_t eltolas = 2;
    if (value >= 10) { eltolas = 4; }
     if (value >= 99) { eltolas = 7; }
    tft->setCursor(centerx-eltolas, centery-3);
    tft->setTextSize(1);
    tft->print16_t(value);

    eltolas =2;
    if (aimValue >= 10) { eltolas = 4; }
    if (aimValue >= 99) { eltolas = 7; }

    tft->fillRoundRect( centerx - 15, centery + 27, 29, 15,3,  tft->color565(30, 30, 120));
    tft->setCursor(centerx-eltolas, centery+30);
    tft->setTextSize(1);
    tft->print16_t(aimValue);
}
*/


/*
void Gauge::DrawSpeedNumber() {

    int16_t eltolas =3;
    if (value >= 10) { eltolas = 5; }
    if (value >= 99) { eltolas = 8; }

    tft->fillRoundRect( centerx - 15, centery - 42, 29, 15,3,  tft->color565(30, 30, 120));
    tft->setCursor(centerx-eltolas, centery-38);
    tft->setTextSize(1);
    tft->print16_t(value);

}
*/


// Draw Arrow
/*
void Gauge::DrawArrow(bool draw) {

    if (gType == Speed) {
        DrawArrowSpeed(draw);
        return;
    }

    uint16_t16_t p1x, p1y, p2x, p2y, p0x, p0y;

    int16_t16_t deg1;
    if (draw) {
		deg1 = map( value, minvalue, maxvalue, MAXDEG, MINDEG);
    } else {
        deg1 = map (oldValue, minvalue, maxvalue, MAXDEG, MINDEG);
    }

    float deg = deg1 *  DEGTORAD;
    p0x = centerx + cos(deg) * (radius2 - 3);
    p0y = centery - sin(deg) * (radius2 - 3);
    deg1 -= 5;

    deg = deg1 * DEGTORAD;
    p1x = centerx + cos(deg) * (radius2 - 20);
    p1y = centery - sin(deg) * (radius2 - 20);

    deg1 += 10;
    deg = deg1 * DEGTORAD;
    p2x = centerx + cos(deg) * (radius2 - 20);
    p2y = centery - sin(deg) * (radius2 - 20);


  /*  if (!first) {

        if (oarrow > arrow) {     // jobbra mozdult a mutatoo
            tft.fillTriangle(o0x, o0y, p0x, p0y, p2x, p2y, BACK);
            tft.fillTriangle(o0x, o0y, o2x, o2y, p2x, p2y, BACK);

        }
        else {
            tft.fillTriangle(o0x, o0y, p0x, p0y, p1x, p1y, BACK);
            tft.fillTriangle(o0x, o0y, o1x, o1y, p1x, p1y, BACK);
        }
    }
    first = false;*/

/*
    uint16_t16_t clr;
    if (draw) {
        clr = tft->color565(255, 255, 200);
    } else {
		clr = theme->FILL1;

    }

    tft->fillTriangle(p1x, p1y, p0x, p0y, p2x, p2y, clr);

}*/


// Draw Temperature gauge
/*
void Gauge::ReDraw()
{
    renderer->AddRectangle(xo,yo,width, height, 1, 0xeeee);

   /* if (this->gType == Speed ) {
        ReDrawSpeed();
        return;
    }*/

/*

    float xd;
    float yd;

    uint16_t16_t p0x, p1x, p2x, p3x;
    uint16_t16_t p0y, p1y, p2y, p3y;

    uint16_t8_t colorr = 255;
    uint16_t8_t colorg;

    #define MINDEG -30
    #define MAXDEG 210

    for (int16_t16_t it = MAXDEG; it > MINDEG; it -= 5)
    {

        xd = cos(float(it) *  DEGTORAD);
        yd = sin(float(it) *  DEGTORAD);

        p0x = centerx + xd * radius1;
        p0y = centery - yd * radius1;

        p1x = centerx + xd * radius2;
        p1y = centery - yd * radius2;

        xd = cos(float(it + 3) * DEGTORAD);
        yd = sin(float(it + 3) * DEGTORAD);

        p2x = centerx + xd * radius1;
        p2y = centery - yd * radius1;

        p3x = centerx + xd * radius2;
        p3y = centery - yd * radius2;

        colorr = 255 - (it + 45) / 1.2;
        colorg = (it + 45) / 1.2;

        renderer->AddTriangle(p0x, p0y, p1x, p1y, p2x, p2y,1);
        renderer->AddTriangle(p2x, p2y, p3x, p3y, p1x, p1y,1);

    }
    // Widget::tft->drawCircle(centerx, centery, radius1 + 2, Widget::tft->color565(100, 100, 100));
    //  Widget::tft->drawCircle(centerx, centery, radius1, Widget::tft->color565(100, 100, 100));

   // DrawTempNumber();
   // DrawArrow(true);

}
*/
