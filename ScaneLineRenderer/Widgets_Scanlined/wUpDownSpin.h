#ifndef UPDOWNSPIN
#define UPDOWNSPIN

#include "widget.h"

class UpDownSpinner : public Widget {
public:
    UpDownSpinner(int x0, int y0, uint8_t width, uint8_t height, uint8_t maxValue, uint8_t id);

    // Widget interface
    virtual void Update(Message message, int mx, int my);
    virtual void ReDraw();

    void    SetValue(uint8_t v);
    uint8_t GetValue() { return value; }
protected:
    void ReDrawButtons();
	void updateValue();	
	bool pressDw, pressUp;	
	bool opressedR, opressDw;

	uint8_t maxValue;
	uint8_t value, oldvalue;
	uint8_t size;
};

#endif
