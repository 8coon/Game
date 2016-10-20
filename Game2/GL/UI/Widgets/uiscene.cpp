#include "uiscene.h"


bool UIScene::paintEvent(UISurface *surface)
{
    context->clear();
    scene->draw(context);
    return true;
}
