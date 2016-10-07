#include "uigridlayout.h"


void UIGridLayout::rearrange(int height, int width)
{
    int left = getOffsetX() + getBorder();
    int top = getOffsetY() + getBorder();
    int wiHeight = height / getRows() - getBorder() * 2;
    int wiWidth = width / getCols() - getBorder() * 2;
    int maxHeight = wiHeight;
    int x = 0;

    for (auto& pair: *this) {
        pair.second->setWidth(wiWidth);
        pair.second->setHeight(wiHeight);
        pair.second->setLeft(left);
        pair.second->setTop(top);
        left += pair.second->getWidth();
        x++;

        if (pair.second->getHeight() > maxHeight) maxHeight = pair.second->getHeight();

        if (x >= getCols()) {
            top += maxHeight + getBorder();
            left = getBorder();
            maxHeight = wiHeight;
            x = 0;
        }
    }
}
