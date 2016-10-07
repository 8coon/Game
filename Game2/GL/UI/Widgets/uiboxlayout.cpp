#include "uiboxlayout.h"


void UIBoxLayout::rearrange(int height, int width)
{
    if (widgets.size() > 0) {
        int size = ((ornt == UI_BOX_HORIZONTAL)?(width - getBorder()*2):(height - getBorder()*2)) / widgets.size();
        int sizeOccupied = 0;
        int i = 0;

        for (auto& pair: *this) {
            i++;
            if (i == (int)widgets.size()) {
                size = ((ornt == UI_BOX_HORIZONTAL)?(width):(height)) - sizeOccupied - getBorder() * 2;
            }

            if (ornt == UI_BOX_HORIZONTAL) {
                pair.second->setTop(getOffsetY() + getBorder());
                pair.second->setLeft(getOffsetX() + sizeOccupied + getBorder());
                pair.second->setHeight(getContainer()->getHeight() - getBorder() * 2);
                pair.second->setWidth(size);
                sizeOccupied += pair.second->getWidth();
            } else {
                pair.second->setTop(getOffsetY() + sizeOccupied + getBorder());
                pair.second->setLeft(getOffsetX() + getBorder());
                pair.second->setHeight(size);
                pair.second->setWidth(getContainer()->getWidth() - getBorder() * 2);
                sizeOccupied += pair.second->getHeight();
            }

            sizeOccupied += getBorder();
        }
    }
}
