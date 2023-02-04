#include "scaneLineRenderer.h"

ScaneLineRenderer::ScaneLineRenderer(unsigned int width):
	objectNumber(0),
	tftWidth(width) {}

void ScaneLineRenderer::RenderObjectsInLine(unsigned int actualLine)
{
	for (unsigned o = 0; o < 320; lineColors[o++] = 0 /*LineDeeps[o++] = 255*/) {};

    for (unsigned i = 0; i < objectNumber; i++) {
       objects[i]->RenderLine(actualLine,this->lineColors, NULL);
    }
}

void ScaneLineRenderer::AddDrawableObj(DrawableObj *obj)
{
    objects[objectNumber++] = obj;
}

void ScaneLineRenderer::RemoveDrawableObj(DrawableObj* objectToRemove) {
	unsigned i = 0;
	for (; i < objectNumber; i++) {
		if (objects[i] == objectToRemove) {
			objectNumber--;
			objects[i] = objects[objectNumber];
			break;
		}
	}
}

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))