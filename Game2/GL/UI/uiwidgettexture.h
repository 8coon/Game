#ifndef UIWIDGETTEXTURE_H
#define UIWIDGETTEXTURE_H

#include "../../SDL2/SDL.h"
#include "uitexture.h"
#include "../../common.h"


using namespace coon;


enum UIWidgetRect { UI_TOPLEFT, UI_TOP, UI_TOPRIGHT, UI_RIGHT, UI_BOTTOMRIGHT, UI_BOTTOM, UI_BOTTOMLEFT, UI_LEFT,
                    UI_CENTER };


class UIWidgetTexture
{
private:
    int left = 0;
    int top = 0;
    Point offset;
    Point size;
    UITexture* texture;
public:
    UIWidgetTexture() {}
    UIWidgetTexture(UITexture* texture, Rect pos, Point size);
    UIWidgetTexture(UITexture* texture, int left = 0, int top = 0):
            UIWidgetTexture(texture, Rect(0, 0, left, top), Point(texture->getWidth(), texture->getHeight())) {}

    UITexture* getTexture() { return texture; }
    Rect getRect(UIWidgetRect rect);
    Rect getAbsRect(UIWidgetRect rect, Point offset, int width, int height);

    int getLeft() { return left; }
    int getTop() { return top; }
    Point getOffset() { return offset; }
    Point getSize() { return size; }
};

#endif // UIWIDGETTEXTURE_H
