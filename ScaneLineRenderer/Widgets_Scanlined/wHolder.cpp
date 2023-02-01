#include "wHolder.h"
#include "widget.h"

WHolder::WHolder(char * name) : MenuItem(name) {
    wnumbers = 0;
}

void WHolder::AddWidget(Widget * wid, ButtonHandler * handler) {
    if (handler != NULL) wid->SetHandler(handler);
    if (wnumbers < MAXW) {
        widgets[wnumbers] =  wid;
        wid->SetParentOrigo(&this->Origo);
        wnumbers++;
    }
    else {
		// error 
    }
}

bool WHolder::UpdateWidgets(Message message, int param1, int param2) {
    for (int i = 0; i < wnumbers; i++) {
        widgets[i]->Update(message, param1 -Origo.x, param2- Origo.y);

    }
    return true;
}

void WHolder::ReDrawWidgets() {
    for (int i = 0; i < wnumbers; i++) {
        widgets[i]->ReDraw();
    }
}

WHolder::~WHolder()
{
}
