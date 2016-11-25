#include "uiscene.h"


bool UIScene::paintEvent(UISurface *surface)
{
    context->clear();
    scene->draw(context);
    return true;
}


void UIScene::setupEvents()
{
    onClick = [](UIWidget* w)
    { ((UIScene*)w)->getActionBinder()->actionClick(); };
    onDblClick = [](UIWidget* w)
    { ((UIScene*)w)->getActionBinder()->actionDblClick(); };
    onMouseMove = [](UIWidget* w, int x, int y)
    { ((UIScene*)w)->getActionBinder()->actionMouseMove(x, y); };
    onMouseDown = [](UIWidget* w, int x, int y)
    { ((UIScene*)w)->getActionBinder()->actionMouseDown(x, y); };
    onMouseUp = [](UIWidget* w, int x, int y)
    { ((UIScene*)w)->getActionBinder()->actionMouseUp(x, y); };
    onMouseDrag = [](UIWidget* w, int x, int y)
    { ((UIScene*)w)->getActionBinder()->actionMouseDrag(x, y); };
    onMouseScroll = [](UIWidget* w, int x)
    { ((UIScene*)w)->getActionBinder()->actionMouseScroll(x); };
    onKeyPress = [](UIWidget* w, int k)
    { ((UIScene*)w)->getActionBinder()->actionKeyPress(k); };
    onKeyRelease = [](UIWidget* w, int k)
    { ((UIScene*)w)->getActionBinder()->actionKeyRelease(k); };
}
