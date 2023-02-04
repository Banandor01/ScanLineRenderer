#pragma once

#ifndef ONPC
#include "../ScaneLineRenderer\drawableObject.h"
#include "../ScaneLineRenderer\scaneLineRenderer.h"
#include "../ScaneLineRenderer\utils.h"
#else
#include "../../ScaneLineRenderer\drawableObject.h"
#include "../../ScaneLineRenderer\scaneLineRenderer.h"
#include "../../ScaneLineRenderer\utils.h"
#endif

class ArtificalHorizon {

public:
	ArtificalHorizon() {}

	void CreateAH(ScaneLineRenderer & renderer);
	void SetValues(ScaneLineRenderer & renderer, float pitch, float roll);

	Line lines[20];
	Line Center1, Center2;

    // for roll indicator
	Line rollLines[10];
	Line rollLines2[20];
	Triangle rollIndTri;

    // Cneter Part
	Triangle centerTri;

	FilledRectWithFrame rect1, rect2;
	FilledRectWithFrame rightRect, leftRect;
	
	//
	Triangle sky, sky2;
	Triangle ground, ground2;
	
	RotatedText rotated, rotated1, rotated2, rotated3, rotated4, rotated5;

    Text text;

	char buffer[5];
	char buffer1[5];
	char buffer2[5];
	char buffer3[6];

	const int16_t deg2Pix = 4;
	const int16_t centerx = 160;
};