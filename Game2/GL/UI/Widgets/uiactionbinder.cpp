#include "uiactionbinder.h"


UIActionBinder::UIActionBinder(UIScene* scene)
{
    setScene(scene);
    
    for (int i = UIKEY_MIN; i < UIKEY_MAX; i++) {
        keys.insert(Pair<UIKey, bool>(i, false));
        reallocs.insert(Pair<UIKey, UIKey>(i, i));
    }
    
    buttons[UIBUTTON_LEFT] = false;
    buttons[UIBUTTON_MIDDLE] = false;
    buttons[UIBUTTON_RIGHT] = false;
    
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_CLICK, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_DBLCLICK, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_MOUSEDOWN, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_MOUSEUP, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_MOUSESCROLL, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_MOUSEDRAG, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_KEYPRESS, Vector<UIActionHandler>()));
    handlers.insert(Pair<UIActionType, Vector<UIActionHandler>>(UIA_KEYRELEASE, Vector<UIActionHandler>()));
}


void UIActionBinder::setRawDown(int keyCode, bool value)
{
    keyCode = reallocs[keyCode];
    
    if ((keyCode >= UIKEY_MIN) && (keyCode < UIKEY_MAX)) {
        keys[keyCode] = value;
    }
}


bool UIActionBinder::isRawDown(int keyCode)
{
    keyCode = reallocs[keyCode];
    
    if ((keyCode >= UIKEY_MIN) && (keyCode < UIKEY_MAX)) {
        return keys[keyCode];
    }
    
    return false;
}


void UIActionBinder::actionPerform(UIActionType type,
                                   int x, int y, int keyCode)
{
    unsigned int buttonState = 0;
    
    if ((x == -1) || (y == -1)) {
        buttonState = SDL_GetMouseState(&x, &y);
    }
    
    if (buttonState == 0) {
        buttonState = SDL_GetMouseState(NULL, NULL);
    }
    
    buttons[UIBUTTON_LEFT] = buttonState & SDL_BUTTON(SDL_BUTTON_LEFT);
    buttons[UIBUTTON_MIDDLE] = buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    buttons[UIBUTTON_RIGHT] = buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT);

    UIAction action(type, this->getScene(), this);
    action.data.coord = Point(x, y);
    
    if (type == UIA_MOUSESCROLL) {
        action.data.coord.y = keyCode;
    } else {
        if (keyCode != -1) {
            action.data.keyCode = reallocs[keyCode];
        }
    }
    
    if (type == UIA_KEYPRESS) setDown(action.data.keyCode, true);
    if (type == UIA_KEYRELEASE) setDown(action.data.keyCode, false);
    
    for (UIActionHandler handler: handlers[type]) {
        handler(&action);
    }
}


void UIActionBinder::bindAction(UIActionType type, UIActionHandler handler)
{
    handlers[type].push_back(handler);
}






