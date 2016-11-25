#ifndef uiactionbinder_hpp
#define uiactionbinder_hpp

#include "../../../common.h"
#include "../../../Containers/containers.h"
#include "../../../Strings/string.h"
#include "../../Scene/scene.h"
#include "../../glcontext.h"
#include "uiwidget.h"
#include "../../Scene/snlandscapenode.h"
#include "../../../SDL2/SDL.h"


#define UIKEY_MIN 4
#define UIKEY_MAX 282


#define   UIA_CLICK         1
#define   UIA_DBLCLICK      2
#define   UIA_MOUSEMOVE     3
#define   UIA_MOUSEDOWN     4
#define   UIA_MOUSEUP       5
#define   UIA_MOUSEDRAG     6
#define   UIA_MOUSESCROLL   7
#define   UIA_KEYPRESS      8
#define   UIA_KEYRELEASE    9

#define UIBUTTON_LEFT    1
#define UIBUTTON_MIDDLE  2
#define UIBUTTON_RIGHT   3



typedef int UIKey;
typedef short UIActionType;
typedef short UIActionButton;


class UIActionBinder;
class UIScene;


union UIActionData {
    Point coord;
    int keyCode;
    
    UIActionData(Point coord = Point(-1, -1), int keyCode = -1)
            { this->coord = coord; this->keyCode = keyCode; }
};


struct UIAction {
    UIActionType type;
    UIScene* scene;
    UIActionBinder* binder;
    UIActionData data;
    
    UIAction(UIActionType type, UIScene* scene, UIActionBinder* binder)
            { this->type = type; this->scene = scene; this->binder = binder; }
};


typedef void (*UIActionHandler)(UIAction const*);


class UIActionBinder: public UIWidget
{
private:
    Map<UIActionType, Vector<UIActionHandler>> handlers;
    Map<UIKey, UIKey> reallocs;
    Map<UIKey, bool> keys;
    Map<UIActionButton, bool> buttons;
    UIScene* scene = NULL;
protected:
    virtual void actionPerform(UIActionType type,
                               int x = -1, int y = -1, int keyCode = -1);
public:
    UIActionBinder(UIScene* scene);
    
    bool isDown(int keyCode) { return isRawDown(reallocs[keyCode]); }
    void setDown(int keyCode, bool value)
            { setRawDown(reallocs[keyCode], value); }
    bool isRawDown(int keyCode);
    void setRawDown(int keyCode, bool value);
    
    bool isMouseDown(UIActionButton btn) { return buttons[btn]; }
    void setMouseDown(UIActionButton btn, bool val) { buttons[btn] = val; }
    
    UIScene* getScene() { return scene; }
    void setScene(UIScene* scene) { this->scene = scene; }
    
    virtual void actionClick()
            { actionPerform(UIA_CLICK); }
    virtual void actionDblClick()
            { actionPerform(UIA_DBLCLICK); }
    virtual void actionMouseMove(int x, int y)
            { actionPerform(UIA_MOUSEMOVE, x, y); }
    virtual void actionMouseDown(int x, int y)
            { actionPerform(UIA_MOUSEDOWN, x, y); }
    virtual void actionMouseUp(int x, int y)
            { actionPerform(UIA_MOUSEUP, x, y); }
    virtual void actionMouseDrag(int x, int y)
            { actionPerform(UIA_MOUSEDRAG, x, y); }
    virtual void actionMouseScroll(int x)
            { actionPerform(UIA_MOUSESCROLL, -1, -1, x); }
    virtual void actionKeyPress(int keyCode)
            { actionPerform(UIA_KEYPRESS, -1, -1, keyCode); }
    virtual void actionKeyRelease(int keyCode)
            { actionPerform(UIA_KEYRELEASE, -1, -1, keyCode); }
    
    void bindAction(UIActionType type, UIActionHandler handler);
};


#endif /* uiactionbinder_hpp */
