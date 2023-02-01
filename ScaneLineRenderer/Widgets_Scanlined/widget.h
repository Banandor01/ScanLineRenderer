#pragma once

#include "../scaneLineRenderer.h"

#define NULL 0

enum Message { ClickDown = 0, Down, ClickUp,
               MouseEventsEnd,   
			   AppendText,
               SetPos,
               GetPos,
               ControlEventsEnd,			  
               NormalUpdate };

class ButtonHandler;
class Point;

class Widget
{
public:
    Widget(int16_t x, int16_t y, int16_t width, int16_t height, int16_t id);

    void SetParentOrigo(Point * origo);

    virtual void Update(Message message, int16_t param1, int16_t param2) = 0;
    virtual void Create() = 0;

    virtual void SetHandler ( ButtonHandler * /* handler*/ ) {}

    static void  SetRenderer(ScaneLineRenderer * renderer);    
    static ScaneLineRenderer * renderer;
  
    uint8_t ID;

protected:     
    Point * parentOrigo;
    int16_t xo, yo;
    int16_t width, height;   
  
};

/*
class Point {
public:
    Point (int16_t x, int16_t y) {
        this->x = x; this->y = y;
    }
    Point () {
        x = 0; y = 0;
    }
    int16_t x;
    int16_t y;
};
*/






