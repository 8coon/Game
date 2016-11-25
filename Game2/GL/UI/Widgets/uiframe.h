#ifndef UIFRAME_H
#define UIFRAME_H


#include "../../../common.h"
#include "uiwidget.h"
#include "../uiskin.h"
#include "../uiwidgettexture.h"


class UIFrame: public UIWidget
{
public:
    UIFrame(UIWidget* parent);

    UIWidgetTexture getWidgetTexture() {
        return getSkin()->getWidgetTexture("FRAME"); }
    bool paintEvent(UISurface *surface);
};

#endif // UIFRAME_H
