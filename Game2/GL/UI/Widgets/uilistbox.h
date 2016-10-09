#ifndef UILISTBOX_H
#define UILISTBOX_H


#include "../../../common.h"
#include "../../../Strings/string.h"
#include "uiwidget.h"
#include "uiscroll.h"


using namespace coon;


class UIListBoxItem: public UIButton
{
private:
    String skinName;
    bool isSelected = false;
protected:
    virtual String selToStr() { return (isSelected)?("_SELECTED"):(""); }
public:
    UIListBoxItem(UIWidget* parent, String caption, String skinName = "LIST_ITEM");

    UIWidgetTexture getWidgetTexture() { return getSkin()->getWidgetTexture(skinName + selToStr()); }

    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() { return isSelected; }
};


class UIListBox: public UIScrollArea
{
private:
    Vector<UIListBoxItem*> items;
    int itemHeight = 30;
    int selIndex = -1;
protected:
    virtual void rearrange();
public:
    UIListBox(UIWidget* parent);

    bool paintEvent(UISurface *surface);
    UIWidgetTexture getWidgetTexture() { return getSkin()->getWidgetTexture("LIST_ITEM"); }

    String getItemAt(int index) { return items[index]->getCaption(); }
    void setItemAt(int index, String item)
        { if (items[index] != NULL) delete items[index]; items[index] = new UIListBoxItem(this, item); rearrange(); }
    int size() { return (int) items.size(); }
    void append(String item) { items.push_back(new UIListBoxItem(this, item)); rearrange(); }
    void insert(int index, String item)
        { items.insert(items.begin() + index, new UIListBoxItem(this, item)); rearrange(); }
    void remove(int index)
        { if (items[index] != NULL) delete items[index]; items.erase(items.begin() + index); rearrange(); }

    int getItemHeight() { return itemHeight; }
    void setItemHeight(int itemHeight) { this->itemHeight = itemHeight; rearrange(); }
    int getSelIndex() { return selIndex; }
    void setSelIndex(int selIndex) { this->selIndex = selIndex; }
};

#endif // UILISTBOX_H
