#include "/../ScaneLineRenderer/scaneLineRenderer.h"
#include <cmath>

ScaneLineRenderer::ScaneLineRenderer(int width) {
    objectNumber = objectNumber2 = 0;
    tftWidth  = width;
}

static  unsigned long rt = 0;
void ScaneLineRenderer::RenderObjectsInLine(unsigned int actualLine)
{
	for (int o = 0; o < 320; LineColors[o++] = 0 /*LineDeeps[o++] = 255*/) {};

    for (int i = 0; i < objectNumber2; i++) {
       objects2[i]->RenderLine(actualLine,this->LineColors, NULL);
    }
}

void ScaneLineRenderer::AddDrawableObj(DrawableObj *obj)
{
    objects2[objectNumber2++] = obj;
}

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
