#ifndef WTRACK_H
#define WTRACK_H

#include "widget.h"

class TrackBar : public Widget
{
public:
    TrackBar (int x0, int y0, int w0, int h0, uint8_t id);

    // Widget interface
    virtual void Update(Message message, int mx, int my);
    virtual void ReDraw();
	virtual void UpdateDraw();

    void    SetValue(uint8_t v);
    uint8_t GetValue() { return value; }

//protected:
    uint8_t value, oldvalue;
	DrawAbleObject * rect;
	DrawAbleObject * rect1;

};

#endif // WTRACK_H
