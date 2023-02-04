
#define SDL_MAIN_HANDLED

#include <iostream> 
#include "VirtualTFT.h"
#include "../../ScaneLineRenderer/scaneLineRenderer.h"

#include "ArtificalHorizon.h"
#include "airspeedindicator.h"

#include <Windows.h>

void Init();
void Loop();

int WinMain( HINSTANCE hInstance,
			 HINSTANCE hPrevInstance,
			 LPSTR     lpCmdLine, int nShowCmd)
{
	Init();
	Loop();

	std::getchar();
}


// ******************************************************************************************************************
// Embedded device memory allocations and functions  

VirtualTFT tft(320, 240);
Triangle triangle(30, 30, 300, 30, 160, 200, 100, 0x00ff);
ScaneLineRenderer renderer(320);
ArtificalHorizon Ahorizon;
AirSpeedIndicator airSpeed;

void Init()
{
	tft.Init();

	//	renderer.AddDrawableObj(&triangle);
    Ahorizon.CreateAH(renderer);
//	airSpeed.Create(renderer);
}

class Application 
{

public:

	Application() : run(true), x(0), y(0) {}

	bool HandleEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch (event.type) {
		case SDL_QUIT:
			run = false;
			break;
		case SDL_KEYUP:
			x = y = 0;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP: y = -0.2f;
				break;
			case SDLK_DOWN: y = +0.2f;
				break;
			case SDLK_LEFT: x = -0.3f;
				break;
			case SDLK_RIGHT: x = 0.3f;
				break;

			default:;
			}
		}
		return true;
	}
	
	int GetPitch() {}
	bool Run() { return run; }
	float X() { return x; }
	float Y() { return y; }
private:
	float x, y;
	bool run;
};

void Loop()
{
	float pitch = 0;
	float roll = 0;

	Application app;


	while (app.Run())
	{
		app.HandleEvents();

		pitch += app.Y();
		roll += app.X();

		if (pitch > 360) { pitch -= 360;}
		if (pitch < -360) { pitch += 360; }
		
		tft.ClearFrame(20, 20, 20);
		
		for (unsigned y = 0; y < 240; y++) {
			renderer.RenderObjectsInLine(y);			
			tft.DrawLineBuffer(renderer.GetColorBuffer(), 320, y);
		}

	//	airSpeed.SetValues(renderer, pitch);
		Ahorizon.SetValues(renderer, pitch, roll);

		tft.UpdateFrame();
	}
}