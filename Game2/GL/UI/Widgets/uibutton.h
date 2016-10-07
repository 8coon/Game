#ifndef UIBUTTON_H
#define UIBUTTON_H


#include "../../../common.h"
#include "uiwidget.h"
#include "../uiskin.h"
#include "../uiwidgettexture.h"


class UIButton: public UIWidget
{
private:
    String caption = "UIButton";
    UITexture* icon = NULL;
    Rect iconRect;
public:
    UIButton(UIWidget* parent);
    UIButton(UIWidget* parent, const String& caption): UIButton(parent) { setCaption(caption); }

    UIWidgetTexture getWidgetTexture();
    bool paintEvent(UISurface *surface);

    String getCaption() { return caption; }
    void setCaption(const String& caption) { this->caption = caption; }
    UITexture* getIcon() { return icon; }
    Rect getIconRect() { return iconRect; }
    void setIcon(UITexture* texture, Rect rect) { icon = texture; iconRect = rect; }
};

#endif // UIBUTTON_H
