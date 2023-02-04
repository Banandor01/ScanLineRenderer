#include "ArtificalHorizon.h"

#ifdef ONPC
#include <cstdlib>
#endif // ONPC

using namespace util;

void ArtificalHorizon::CreateAH(ScaneLineRenderer& renderer) {

	// Sky
	renderer.AddDrawableObj(&sky);
	renderer.AddDrawableObj(&sky2);
	// Ground
	renderer.AddDrawableObj(&ground);
	renderer.AddDrawableObj(&ground2);

	// horizon
	lines[0].SetLine(0, 0, 0, 0, 1, 0xffff);
	lines[1].SetLine(0, 0, 0, 0, 1, 0xffff);
	renderer.AddDrawableObj(&lines[0]);
	renderer.AddDrawableObj(&lines[1]);

	// pitch lines
	int o = 2;
	for (int i = -30; i <= 30; i += 5, o++) {
		renderer.AddDrawableObj(&lines[o]);
	}

	// center indicator
	rect1 = FilledRectWithFrame(centerx - 70, 120, 40, 3, 1, 0xffff, 0x0000, false);
	rect2 = FilledRectWithFrame(centerx + 30, 120, 40, 3, 1, 0xffff, 0x0000, false);
	renderer.AddDrawableObj(&rect1);
	renderer.AddDrawableObj(&rect2);

	// roll indicator
	uint16_t color = color565(255, 255, 255);
	o = 0;
	for (int i = -60; i <= 60; i += 15) {

		int d = 0;
		if (i == 0 || i == -60 || i == 60) {
			d = 7;
		}

		Point t1(0, 112 + d);
		Point t2(0, 102);
		//	Point t3(0, 100);

		Rotate(t1, i);
		Rotate(t2, i);
		//	Rotate(t3, i);

		renderer.AddDrawableObj(&rollLines[o]);
		rollLines[o].SetLine(t1.x + centerx, 120 - t1.y,
			t2.x + centerx, 120 - t2.y, 1, color);

		o++;
	}

	o = 0;
	Point old(0, 102);
	Rotate(old, -60);
	for (int i = -60; i <= 60; i += 8) {
		Point t1(0, 102);
		Rotate(t1, i);

		renderer.AddDrawableObj(&rollLines2[o]);
		rollLines2[o].SetLine(old.x + centerx, 120 - old.y,
			t1.x + centerx, 120 - t1.y, 1, color);
		old = t1;
		o++;
	}

	// Center part
	rightRect = FilledRectWithFrame(0, 110, 40, 20, 1, color565(100, 100, 100), 0xffff, true);
	leftRect = FilledRectWithFrame(290, 110, 30, 20, 1, color565(100, 100, 100), 0xffff, true);
	renderer.AddDrawableObj(&rightRect);
	renderer.AddDrawableObj(&leftRect);

	centerTri = Triangle(centerx - 20, 130, centerx, 120, centerx + 20, 130, 1, 0xffff);

	renderer.AddDrawableObj(&centerTri);
	renderer.AddDrawableObj(&rollIndTri);

	renderer.AddDrawableObj(&rotated);
	renderer.AddDrawableObj(&rotated1);
	renderer.AddDrawableObj(&rotated2);
	renderer.AddDrawableObj(&rotated3);
	renderer.AddDrawableObj(&rotated4);
	renderer.AddDrawableObj(&rotated5);
	renderer.AddDrawableObj(&text);

}

void ArtificalHorizon::SetValues(ScaneLineRenderer& renderer, float pitch, float roll)
{
	if (pitch >= 180) {
		pitch -= 360;
	}
	else if (pitch <= -180) {
		pitch += 360;
	}


	if (pitch > 90) {
		pitch = 180 - pitch;
		roll += 180;
	}

	if (pitch < -90) {
		pitch = -180 - pitch;
		roll += 180;
	}
	

	const int wide = 50;

#ifdef ONPC
	_itoa_s(pitch, buffer1, 10);
#else
	itoa(pitch, buffer1, 10);
#endif
	text.SetValue(3, 113, 2, buffer1, 1, 0xffff);

	Point rotateV(100, 0);
	Rotate(rotateV, roll);

	// roll indicator
	Point t1(-6, -82);	Point t2(6, -82);
	Point t3(0, -105);

	Rotate(t1, rotateV);    Rotate(t2, rotateV);
	Rotate(t3, rotateV);

	rollIndTri.SetValue(t1.x + centerx, 120 + t1.y, t2.x + centerx, 120 + t2.y,
		t3.x + centerx, 120 + t3.y, 1, color565(255, 200, 200));

	Point t4;

	// sky and ground

	t1.x = -160; t1.y = pitch * deg2Pix;
	t2.x = 160; t2.y =  pitch * deg2Pix;
	Rotate(t1, roll);    Rotate(t2, roll);

	t3.x = -160;  t3.y = (-180 + pitch) * deg2Pix;
	t4.x = 160;   t4.y = (-180 + pitch) * deg2Pix;

	Rotate(t3, roll);    Rotate(t4, roll);

	sky.SetValue(t1.x + centerx, 120 + t1.y,
		t2.x + centerx, 120 + t2.y,
		t3.x + centerx, 120 + t3.y, 1, color565(50, 50, 200));

	sky2.SetValue(t2.x + centerx, 120 + t2.y,
		t3.x + centerx, 120 + t3.y,
		t4.x + centerx, 120 + t4.y, 1, color565(50, 50, 200));


	t3.x = -160;  t3.y = (+180 + pitch) * deg2Pix;
	t4.x = 160;   t4.y = (+180 + pitch) * deg2Pix;

	Rotate(t3, roll);    Rotate(t4, roll);

	ground.SetValue(t1.x + centerx, 120 + t1.y, t2.x + centerx, 120 + t2.y,
		t3.x + centerx, 120 + t3.y, 1, 0x4444);

	ground2.SetValue(t3.x + centerx, 120 + t3.y, t2.x + centerx, 120 + t2.y,
		t4.x + centerx, 120 + t4.y, 1, 0x4444);


	// HorizontalLine
	for (int i = 0; i < 2; i++) {

		Point leftP(-160, pitch * deg2Pix - 1 + i);
		Point rightP(160, pitch * deg2Pix - 1 + i);

		Rotate(leftP, roll);
		Rotate(rightP, roll);

		lines[i].SetLine(leftP.x + centerx, 120 + leftP.y,
			rightP.x + centerx, 120 + rightP.y, 0, 0xffff);
	}

	// Pitch indicator lines
	int temppitch = pitch * deg2Pix;
	int shift = temppitch % 40;
	int o = 2;
	for (int i = -30; i <= 30; i += 5, o++) {

		int w = 20;
		if ((i + 100) % 10 == 0) w = wide;

		uint16_t color = color565(150, 255, 150);
		if (pitch - i - shift / 4 < 0) color = color565(0, 255, 255);

		Point leftP(-w, (i * deg2Pix + shift));
		Point rightP(w, (i * deg2Pix + shift));

		Rotate(leftP, rotateV);
		Rotate(rightP, rotateV);

		lines[o].SetLine(leftP.x + centerx, 120 + leftP.y,
			rightP.x + centerx, 120 + rightP.y, 0, color);


		int pitch2 = (((int)(pitch - i)) /10) * 10;


		if (i == 0) {

#ifdef ONPC
			_itoa_s(pitch2, buffer3, 10);
#else
			itoa(pitch2, buffer3, 10);
#endif
			rotated4.SetValue(leftP.x + centerx, 120 + leftP.y, roll, (char*)buffer3);
			rotated5.SetValue(rightP.x + centerx, 120 + rightP.y, roll, (char*)buffer3);
		}


		if (i == 10) {
#ifdef ONPC
			_itoa_s(pitch2, buffer, 10);
#else
			itoa(pitch2, buffer, 10);
#endif
			rotated.SetValue(leftP.x + centerx, 120 + leftP.y, roll, (char*)buffer);
			rotated1.SetValue(rightP.x + centerx, 120 + rightP.y, roll, (char*)buffer);
		}

		if (i == -10) {
#ifdef ONPC
			_itoa_s(pitch2, buffer2, 10);
#else 
			itoa(pitch2, buffer2, 10);
#endif
			rotated2.SetValue(leftP.x + centerx, 120 + leftP.y, roll, (char*)buffer2);
			rotated3.SetValue(rightP.x + centerx, 120 + rightP.y, roll, (char*)buffer2);
		}
	}
}
