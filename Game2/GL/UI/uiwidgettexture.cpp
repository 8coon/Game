#include "uiwidgettexture.h"

UIWidgetTexture::UIWidgetTexture(UITexture* texture, Rect pos, Point size)
{
    this->texture = texture;
    this->size = size;
    left = pos.w;
    top = pos.h;
    offset.x = pos.x;
    offset.y = pos.y;
}


Rect UIWidgetTexture::getRect(UIWidgetRect rect)
{
    return getAbsRect(rect, offset, size.x, size.y);
}


Rect UIWidgetTexture::getAbsRect(UIWidgetRect rect, Point offset, int width, int height)
{
    switch (rect) {
        case UI_TOPLEFT:
            return Rect(offset.x + 0, offset.y + 0, left, top);
        case UI_TOP:
            return Rect(offset.x + left, offset.y + 0, width - left*2, top);
        case UI_TOPRIGHT:
            return Rect(offset.x + width - left, offset.y + 0, left, top);
        case UI_RIGHT:
            return Rect(offset.x + width - left, offset.y + top, left, height - top*2);
        case UI_BOTTOMRIGHT:
            return Rect(offset.x + width - left, offset.y + height - top, left, top);
        case UI_BOTTOM:
            return Rect(offset.x + left, offset.y + height - top, width - left*2, top);
        case UI_BOTTOMLEFT:
            return Rect(offset.x + 0, offset.y + height - top, left, top);
        case UI_LEFT:
            return Rect(offset.x + 0, offset.y + top, left, height - top*2);
        case UI_CENTER:
            return Rect(offset.x + left, offset.y + top, width - left*2, height - top*2);
    }
}
