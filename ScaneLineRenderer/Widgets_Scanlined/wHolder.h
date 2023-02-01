#pragma once

#include "widget.h"
#include "menubar.h"

#define MAXW 15
#define NULL 0

class ButtonHandler;

class WHolder : public MenuItem
{
public:
	WHolder(char* str);
    ~WHolder();
    // WObject interface
    Tipe GetType() { return MenuItem::Tipe::WidgetHolder; }

    void AddWidget(Widget * wid, ButtonHandler * handler = NULL);
    bool UpdateWidgets(Message message, int param1, int param2);
    void ReDrawWidgets();

    Point Origo;


private:
	Widget * widgets[MAXW];
	uint8_t wnumbers;



};

