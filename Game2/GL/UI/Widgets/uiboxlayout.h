#ifndef UIBOXLAYOUT_H
#define UIBOXLAYOUT_H


#include "../../../common.h"
#include "../../../Containers/containers.h"
#include "uiwidget.h"


using namespace coon;


enum UIBoxOrientation { UI_BOX_HORIZONTAL, UI_BOX_VERTICAL };


class UIArrangeableLayout: public UILayout
{
protected:
    virtual void rearrange(int height, int width) = 0;
public:
    void addWidget(UIWidget* widget) { this->UILayout::addWidget(widget);
                                       rearrange(getContainer()->getHeight(), getContainer()->getWidth()); }
    void removeWidget(const String& name) { this->UILayout::removeWidget(name);
                                          rearrange(getContainer()->getHeight(), getContainer()->getWidth()); }
    bool resizeEvent(UIResizeEvent event) { rearrange(event.height, event.width); return true; }
};


class UIBoxLayout: public UIArrangeableLayout
{
private:
    UIBoxOrientation ornt;
    int border = 0;
    Point offset;
protected:
    void rearrange(int height, int width);
public:
    UIBoxLayout(UIBoxOrientation ornt) { this->ornt = ornt; }

    int getBorder() { return border; }
    void setBorder(int border) { this->border = border; }
    Point getOffset() { return offset; }
    void setOffset(Point offset) { this->offset = offset; }
    int getOffsetX() { return offset.x; }
    void setOffsetX(int x) { offset.x = x; }
    int getOffsetY() { return offset.y; }
    void setOffsetY(int y) { offset.y = y; }
    UIBoxOrientation getOrientation() { return ornt; }
};


class UIHBoxLayout: public UIBoxLayout
{
public:
    UIHBoxLayout(): UIBoxLayout(UI_BOX_HORIZONTAL) {}
};


class UIVBoxLayout: public UIBoxLayout
{
public:
    UIVBoxLayout(): UIBoxLayout(UI_BOX_VERTICAL) {}
};



#endif // UIBOXLAYOUT_H
