#include "utils.h"
#include <math.h>

void util:: Rotate(Point & p, float angle) {
    float cosValue = cos(angle * DEG2RAD);
    float sinValue = sin(angle * DEG2RAD);

    int16_t nx =  cosValue * p.x - sinValue *p.y;
    int16_t ny =  sinValue * p.x + cosValue *p.y;

    p.x = nx;   p.y = ny;
}

void util::Rotate(Point & p, const Point & vector)
 {
    int16_t nx = vector.x * p.x - vector.y *p.y;
    int16_t ny = vector.y * p.x + vector.x *p.y;
    p.x = nx / 100;    p.y = ny / 100;
}

uint16_t util::color565(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
#ifndef ONPC
    color = (color >> 8) | ((color << 8) & 0xff00);
#endif
    return color;
}