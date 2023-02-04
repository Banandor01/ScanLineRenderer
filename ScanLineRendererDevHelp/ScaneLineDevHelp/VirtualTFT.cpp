#include "VirtualTFT.h"

bool VirtualTFT::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	int result = SDL_CreateWindowAndRenderer(100 + _width * 2, 100 + _height * 2, NULL, &_window, &_renderer);
	if (result != 0) {
		Log("Failed to create window and renderer:", 1);
		return false;
	}

	uiPixels = new Uint32[(100+_width *2)*(_height * 2 +100)];
	uiTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
				_width *2 +100, _height *2 + 100);

	return true;
}

void VirtualTFT::ClearFrame(Uint8 r, Uint8 g, Uint8 b)
{
	int pitch = 0;
	lockedPixels = nullptr;
	if (!SDL_LockTexture(uiTexture, NULL, (void**)&lockedPixels, &pitch)) {
		// lockedPixels now points to driver-provided memory that we can write to that the driver will then copy to uiTexture
	   // can't ignore pitch, it might be wider than UI_WIDTH
	}
}

void VirtualTFT::UpdateFrame() 
{
	SDL_UnlockTexture(uiTexture);
	
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = _width *2 +100;
	rect.h = _height *2 + 100;

	SDL_RenderCopy(_renderer, uiTexture, &rect, &rect);
	SDL_RenderPresent(_renderer);
}

void VirtualTFT::DrawPixel(unsigned int x, unsigned int y, Uint8 r, Uint8 g, Uint8 b)
{
	Uint32 c = r << 8 | g;
	c = c << 8 | b;
	y *= 2;
	x *= 2;
	y += 50;
	x += 50;

	unsigned y1 = y  * (_width *2  +100);

	lockedPixels[y1 + x] = c | 0xff00000;
	lockedPixels[y1 + x+1] = c | 0xff000000;

	y++;
	y1 = y * (_width * 2 + 100);

	lockedPixels[y1 + x] = c | 0xff000000;
	lockedPixels[y1 + x+1] = c | 0xff000000;
}

void VirtualTFT::DrawLineBuffer(Uint16* colorB, unsigned width, unsigned lineNumber) {
	for (unsigned x = 0; x < width; x++)
	{
		Uint8 r = colorB[x] >> 8 | 0b00000111;
		Uint8 g = (colorB[x] >> 2) | 0b00000011;
		Uint8 b = colorB[x] << 3 | 0b00000111;
		DrawPixel(x, lineNumber, r, g, b);
	}
}

#define swap(x, y) { int temp = x; x = y; y = temp; }


void VirtualTFT::DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	int16_t x2, int16_t y2, Uint8 r, Uint8 g, Uint8 b)
{
	DrawLine(x0, y0, x1, y1, r, g, b);
	DrawLine(x1, y1, x2, y2, r, g, b);
	DrawLine(x2, y2, x0, y0, r, g, b);
}

void VirtualTFT::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Uint8 r, Uint8 g, Uint8 b) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			DrawPixel(y0, x0, r, g, b);
		}
		else {
			DrawPixel(x0, y0, r, g, b);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void VirtualTFT::DrawFastHLine(unsigned int x, unsigned int y, int w, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(_renderer, r, g, b, 255);

	SDL_Rect rect;
	rect.x = 50 + x * 2;
	rect.y = 50 + y * 2;
	rect.h = 2;
	rect.w = w * 2;

	SDL_RenderDrawRect(_renderer, &rect);
}

void VirtualTFT::FillTriangle(int x0, int y0, int x1, int y1,
	int x2, int y2, Uint8 r, Uint8 g, Uint8 blue) {

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		swap(y0, y1);
		swap(x0, x1);
	}
	if (y1 > y2) {
		swap(y2, y1);
		swap(x2, x1);
	}
	if (y0 > y1) {
		swap(y0, y1);
		swap(x0, x1);
	}

	if (y0 < 0) y0 = 0;

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;

		DrawFastHLine(a, y0, b - a + 1, r, g, blue);

		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
		dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b) { swap(a, b) };

		DrawFastHLine(a, y, b - a + 1, r, g, blue);
		//writeFastHLine(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t)dx12 * (y - y1);
	sb = (int32_t)dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b) swap(a, b);

		DrawFastHLine(a, y, b - a  + 1, r, g, blue);
		//writeFastHLine(a, y, b - a + 1, color);
	}
}

