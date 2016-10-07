#include "uiframe.h"

UIFrame::UIFrame(UIWidget* parent): UIWidget(parent)
{
    /*getSkin()->getFont()->addColor("r", RGBA(240, 0, 0, 200));
    getSkin()->getFont()->addColor("g", RGBA(0, 240, 0, 200));
    getSkin()->getFont()->addColor("b", RGBA(0, 0, 240, 200));
    getSkin()->getFont()->setColor(RGBA(255, 255, 255, 200));*/
}


bool UIFrame::paintEvent(UISurface* surface)
{
    UIWidgetTexture texture = getWidgetTexture();
    surface->drawWidget(texture, getHeight(), getWidth());

    //surface->drawText(getSkin()->getFont(), "&rThe &gQuick &bBrown &rFox &gJumps &bOver &ra &gLazy &bDog", Rect(15, 15, 500, 500));
    //surface->drawText(getSkin()->getFont(), "Съешь ещё этих французских булок, да выпей чаю", Rect(15, 35, 500, 500));

    this->UIWidget::paintEvent(surface);
    return true;
}
