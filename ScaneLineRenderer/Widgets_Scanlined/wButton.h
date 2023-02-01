#ifndef TFTBUTTON
#define TFTBUTTON

#include <stdint.h>
#include "widget.h"

class Button : public Widget{
   
public: 
    Button(int x0, int y0, int w0, int h0, uint8_t id, char * text1 = 0);

    virtual void Update(Message message, int mx, int my);
    virtual void ReDraw();
	virtual void UpdateDraw();

    void SetHandler ( ButtonHandler * handler1 ) {   handler = handler1;  }
    void SetState (bool newstae);
    bool manualReset;

protected: 
   ButtonHandler * handler;
   char * text;
   bool pressed;

   DrawAbleObject * doRect;
   DrawAbleObject * doRect1;
   DrawAbleObject * doText;

};

class ButtonHandler {
public:
  virtual void CallBack1 (bool /*pressed */, uint8_t /* id */) {}
  virtual void CallBack2 (bool /*pressed */, uint8_t /* id */) {}
};

#endif

