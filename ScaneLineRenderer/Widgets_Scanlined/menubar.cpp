#include "menubar.h"
#include "WHolder.h"
#include <Arduino.h>

MenuItem::MenuItem() {   
    name = (char*)"-";
}

MenuItem::MenuItem(const char * str)
{
   name = str;
   numberOfChildren = 0;
}

void MenuItem::AddChild(MenuItem * item) {
    if (numberOfChildren < MAXCHILDREN) {
        children[numberOfChildren] = item;
        numberOfChildren++;
    }
}

MenuItem::Tipe MenuItem::GetType () {
    return Tipe::Menu;
}

/*void MenuItem::AddChildren(WObject **menusp, uint8_t cnumber)
{
    children = menusp;
    numberOfChildren = cnumber;
}*/

//#define WMENU 80
#define HMENU 40
//*************************************************************************************************************
MenuBar::MenuBar(int x, int y, int w, int h) :
    Widget (x,y,w,h, 255),
    RootItem((char*)"Main")
{   
    m_actualItem = &RootItem;
    level = 0;
}
//*************************************************************************************************************
void MenuBar::Update(Message message, int mx, int my)
{
    actualItemChanged = false;

    if (message == ClickDown) {
        uint8_t index = 0;
        if (mx > xo && mx < (xo + width)) {
            index = my / HMENU;          
			uint8_t childrenNumber = m_actualItem->numberOfChildren;
            if ( index >= childrenNumber) {              
                if (index == childrenNumber ) {       // back
                    level -=2;
                    if (level < 0) {
                        m_actualItem = &RootItem;
                        level = 0;
                    } else {
                        m_actualItem = menusStack[level];
                    }
					ReDrawPushed(childrenNumber, "");
					delay(200);
                    ReDraw();   
					actualItemChanged = true;
                }
                return;
            }

            if (m_actualItem->numberOfChildren > 0) {
                actualItemChanged = true;
                m_actualItem = m_actualItem->children[index];
				ReDrawPushed(index, "");
				delay(200);
                ReDraw();
                menusStack[level] = m_actualItem;
                level++;
            } 
        }
    }
}

//**************************************************************************************************************
void MenuBar::ReDraw() {
  //  tft->fillRect(0,0, width+1, height, theme->BACK);

	uint8_t i;
    for (i = 0; i < m_actualItem->numberOfChildren; i++) {
//		theme->drawBox(tft, 1, 1 + i * 40, width, HMENU - 2, false);     
    //    tft->drawCentreString((char*)m_actualItem->children[i]->name, width  / 2, 9 + i * 40, 2);
    }   

	if (m_actualItem != &RootItem) {
//		theme->drawBox(tft, 1, 1 + i * 40, width, HMENU - 2, false);
	//	tft->drawCentreString((char*)"BACK", width / 2, 9 + i * 40, 2);
	}
}


void MenuBar::ReDrawPushed(uint8_t index, char * text ) {

//	theme->drawBox(tft, 1, 1 + index * 40, width, HMENU - 2, true);
//	tft->drawCentreString(text, width / 2, 9 + index * 40, 2);
	
}


//*************************************************************************************************************
bool MenuBar::ActualItemChanged()
{
    return actualItemChanged;
}

char * MenuBar::GetFullPath()
{
	uint8_t index = 0;
	path[index++] = '\\';
/*	for (uint8_t i = 0; i < level; i++) {
		char * p = menusStack[i]->name;
		do {
			path[index++] = *p++;
		} while (*p != NULL);
		path[index++] = '\\';
	}*/
	path[index] = 0;
	return path;
}
