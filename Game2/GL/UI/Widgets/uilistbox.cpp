#include "uilistbox.h"


UIListBoxItem::UIListBoxItem(UIWidget *parent, String caption, String skinName): UIButton(parent, caption)
{
    this->skinName = skinName;
}


UIListBox::UIListBox(UIWidget *parent): UIScrollArea(parent)
{
    setScrollBars(UI_VERTICAL);
}


bool UIListBox::paintEvent(UISurface *surface)
{
    rearrange();
    return this->UIScrollArea::paintEvent(surface);
}


static void UIListBox_onClick(UIWidget* sender)
{
    ((UIListBox*) sender->getTag())->setSelIndex(sender->getZOrder());
    ((UIListBox*) sender->getTag())->notify();
}


void UIListBox::rearrange()
{
    getContent()->getLayout()->clear(true);

    for (int i = 0; i < size(); i++) {
        items[i]->setZOrder(i);
        items[i]->setMinHeight(getItemHeight());
        items[i]->setMaxHeight(items[i]->getMinHeight());
        items[i]->setHeight(items[i]->getMaxHeight());
        items[i]->setTag(this);
        
        items[i]->onClick = UIListBox_onClick;

        getContent()->getLayout()->addWidget(items[i]);
    }
}

