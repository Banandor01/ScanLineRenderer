#pragma once

#define SDL_MAIN_HANDLED

#include <iostream> 
#include "SDL.h"

#include "ILogger.h"

class VirtualTFT
{
public:

    VirtualTFT(unsigned width, unsigned height, ILogger * logger = nullptr) :
        _height(height),
        _width(width)
    {
        _logger = logger;
    }

    bool Init();

    void DrawPixel(unsigned int x, unsigned int y, Uint8 r, Uint8 g, Uint8 b);
    void DrawLineBuffer(Uint16* colorB, unsigned width, unsigned lineNumber);
    
    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Uint8 r, Uint8 g, Uint8 b);
    
    
    void DrawFastHLine(unsigned int x, unsigned int y, int w, Uint8 r, Uint8 g, Uint8 b);
    
    void DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, Uint8 r, Uint8 g, Uint8 b);
    
    void FillTriangle(int x0, int y0, int x1, int y1,
                      int x2, int y2, Uint8 r, Uint8 g, Uint8 blue);
       
    void ClearFrame(Uint8 r, Uint8 g, Uint8 b);

    void UpdateFrame();

    unsigned Height() { return _height; }
    unsigned Width() { return _width; }


private:
    void Log(const char* msg, int level) {
        if (_logger != nullptr) {
            _logger->Log(msg, level);
        }
    }

    ILogger* _logger;

    unsigned int _width;
    unsigned int _height;
    
    SDL_Window*   _window;
    SDL_Renderer* _renderer;
    SDL_Surface*  _surface;

    Uint32 * uiPixels;
    SDL_Texture* uiTexture;
    Uint32* lockedPixels = nullptr;


};


