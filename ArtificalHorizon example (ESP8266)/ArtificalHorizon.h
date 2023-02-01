#pragma once

#ifndef ONPC
#include "../ScaneLineRenderer\drawableObject.h"
#include "../ScaneLineRenderer\scaneLineRenderer.h"
#include "../ScaneLineRenderer\utils.h"
#else
#include "../ScaneLineRenderer\drawableObject.h"
#include "../ScaneLineRenderer\scaneLineRenderer.h"
#include "../ScaneLineRenderer\utils.h"
#endif

class ArtificalHorizon {

public:
	ArtificalHorizon() {}

	void CreateAH(ScaneLineRenderer & renderer);
	void SetValues(ScaneLineRenderer & renderer, float pitch, float roll);

	Line lines[20];
	Line Center1;
	Line Center2;

    // for roll indicator
	Line rollLines[10];
	Line rollLines2[20];
	Triangle rollIndTri;

    // Cneter Part
	Triangle centerTri;

	FilledRectWithFrame rect1;
	FilledRectWithFrame rect2;

	FilledRectWithFrame rightRect;
	FilledRectWithFrame leftRect;
	
	//
	Triangle sky;
	Triangle ground;	
	
    RotatedText rotated;
    RotatedText rotated1;
    RotatedText rotated2;
    RotatedText rotated3;

    Text text;

	char buffer[5];
	char buffer1[5];
	char buffer2[5];

	const int16_t deg2Pix = 4;
	const int16_t centerx = 178;
};



