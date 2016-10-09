#include "uibutton.h"

UIButton::UIButton(UIWidget *parent): UIWidget(parent)
{
}


UIWidgetTexture UIButton::getWidgetTexture()
{
    if (!getEnabled()) return getSkin()->getWidgetTexture("BUTTON_GRAYED");

    switch (getState()) {
        case UI_NORMAL: return getSkin()->getWidgetTexture("BUTTON"); break;
        case UI_HOVER: return getSkin()->getWidgetTexture("BUTTON_HOVER"); break;
        case UI_PRESSED: return getSkin()->getWidgetTexture("BUTTON_PRESSED"); break;
        case UI_GRAYED: return getSkin()->getWidgetTexture("BUTTON_GRAYED");
    }

    return getSkin()->getWidgetTexture("BUTTON");
}


bool UIButton::paintEvent(UISurface *surface)
{
    UIWidgetTexture texture = getWidgetTexture();
    surface->drawWidget(texture, getHeight(), getWidth());

    String str = caption;
    Rect rect = getSkin()->getFont()->measureText(str);

    if (rect.w > getWidth() - 10) {
        for (int i = (int) caption.size() - 4; i >= 1; i--) {
            str = caption.substring(0, i);
            str.append("...");

            Rect newRect = getSkin()->getFont()->measureText(str);
            if (newRect.w <= getWidth() - 10) break;
        }
    }

    rect = getSkin()->getFont()->measureText(str);
    Rect center = Rect(getWidth() / 2 - rect.w / 2, getHeight() / 2 - rect.h / 2, rect.w, rect.h);
    surface->drawText(getSkin()->getFont(), str, center);

    this->UIWidget::paintEvent(surface);
    return true;
}
