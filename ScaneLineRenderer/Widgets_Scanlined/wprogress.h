#ifndef WPROGRESS_H
#define WPROGRESS_H

#include "widget.h"
//#include "../ScaneLineLibraray/scanlinedtft.h"

class Progress : public Widget
{
public:
    Progress (int x0, int y0, int w0, int h0, uint8_t id);

    // Widget interface
    void Update(Message message, int param1, int param2);
    void ReDraw();
	virtual void UpdateDraw();
	void SetValue(uint8_t v);

protected:
    uint8_t value;
	DrawAbleObject * rect;
	DrawAbleObject * inrect;

};

#endif // WPROGRESS_H
