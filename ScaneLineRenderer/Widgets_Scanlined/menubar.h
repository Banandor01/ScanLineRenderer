#ifndef MENUBAR_H
#define MENUBAR_H

#include "widget.h"
#include "string.h"

using namespace std;

#define MAXCHILDREN 5

class MenuItem
{
private:
    MenuItem();
public:
    enum Tipe { Menu, WidgetHolder };
    MenuItem (const char * str);

    MenuItem::Tipe virtual GetType();
    void AddChild(MenuItem * item);

    MenuItem * children[MAXCHILDREN];
    uint8_t numberOfChildren;
    const char * name;
};

class MenuBar : public Widget {
public:
    MenuBar(int x, int y, int w, int h);

    // Widget interface
    void Update(Message message, int mx, int my);
    void ReDraw();   
    void AddChildMenu(MenuItem * item) {
             RootItem.AddChild(item);
         }

    bool   ActualItemChanged ();
    char * GetFullPath();
    MenuItem * GetActualItem() {
        return m_actualItem;
    }

private:

	void ReDrawPushed(uint8_t index, char * text);
    MenuItem RootItem;
    MenuItem * m_actualItem;
    MenuItem * menusStack[5];
    int8_t   level;
    bool  actualItemChanged;
    char  path[30];
};

#endif // MENUBAR_H
