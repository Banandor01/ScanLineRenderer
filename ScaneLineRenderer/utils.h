#ifndef UTILS_H
#define UTILS_H

#include "drawableObject.h"

namespace util {

const float DEG2RAD = 3.14159f / 180.0f;

uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

void Rotate(Point & p, float angle);
void Rotate(Point & p, const Point & vector);

}
#endif // UTILS_H
