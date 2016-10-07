#ifndef UISCROLL_H
#define UISCROLL_H


#include "../../../common.h"
#include "../../../Strings/string.h"
#include "uiwidget.h"
#include "uibutton.h"
#include "uiboxlayout.h"
#include "uigridlayout.h"


using namespace coon;


#define SCROLL_BUTTON_SIZE 8
#define SCROLL_BAR_SIZE 30


class UIScrollBoxButtonUp: public UIButton
{
private:
    UIBoxOrientation ornt;
public:
    UIScrollBoxButtonUp(UIWidget* parent, UIBoxOrientation ornt): UIButton(parent, " ") {
        setMinHeight(SCROLL_BUTTON_SIZE);
        setMinWidth(SCROLL_BUTTON_SIZE);
        setMaxHeight(SCROLL_BUTTON_SIZE);
        setMaxWidth(SCROLL_BUTTON_SIZE);
        this->ornt = ornt;
    }

    UIWidgetTexture getWidgetTexture();
    UIBoxOrientation getOrientation() { return ornt; }
    String orntToString() { return (ornt == UI_BOX_HORIZONTAL)?("HR"):("VR"); }
    String buildString(const String& name, const String& status)
        { StringBuilder b; b.append(name); b.append(orntToString()); b.append(status); return b.toString(); }
};


class UIScrollBoxButtonDown: public UIScrollBoxButtonUp
{
public:
    UIScrollBoxButtonDown(UIWidget* parent, UIBoxOrientation ornt): UIScrollBoxButtonUp(parent, ornt) {}
    UIWidgetTexture getWidgetTexture();
};


class UIScrollBoxBar: public UIScrollBoxButtonUp
{
public:
    UIScrollBoxBar(UIWidget* parent, UIBoxOrientation ornt): UIScrollBoxButtonUp(parent, ornt) {}
    UIWidgetTexture getWidgetTexture();
};



class UIScrollBox: public UIButton
{
private:
    UIScrollBoxButtonUp* buttonUp;
    UIScrollBoxButtonDown* buttonDown;
    UIWidget* barContainer;
    UIScrollBoxBar* bar;
    UIBoxOrientation ornt;
    int min = 0;
    int max = 10;
    double dpos = 0;
    Point last;
private:
    String orntToString() { return (ornt == UI_BOX_HORIZONTAL)?("HR"):("VR"); }
    String buildString(const String& name, const String& status)
        { StringBuilder b; b.append(name); b.append(orntToString()); b.append(status); return b.toString(); }
protected:
    virtual void rearrange();
public:
    UIScrollBox(UIWidget* parent, UIBoxOrientation ornt);
    UIWidgetTexture getWidgetTexture();

    UIBoxOrientation getOrientation() { return ornt; }
    int getMin() { return min; }
    void setMin(int v) { min = v; addPos(0); rearrange(); }
    int getMax() { return max; }
    void setMax(int v) { max = v; addPos(0); rearrange(); }
    int getPos() { return round(dpos); }
    double getDoublePos() { return dpos; }
    void setDoublePos(double v)
        { if (v > max) v = max; if (v < min) v = min; dpos = v; if (onChange != NULL) onChange(this); }
    void addDoublePos(double v) { setDoublePos(getDoublePos() + v); }
    void setPos(int v) { setDoublePos(v); }
    void addPos(int v) { addDoublePos(v); }

    bool resizeEvent(UIResizeEvent event);
    double getPrice();
    int getBarContainerSize();
    void setLastMousePos(const Point& point) { last = point; }
    Point getLastMousePos() { return last; }
    void mouseScroll(int v) { setLastMousePos(Point(0, 0)); bar->onMouseDrag(bar, v, v); }
};


enum UIScrollBars { UI_HORIZONTAL, UI_VERTICAL, UI_BOTH, UI_NONE };


class UIScrollArea: public UIWidget
{
private:
    UIWidget* content;
    UIWidget* container;
    UIScrollBox* hScroll;
    UIScrollBox* vScroll;
    UIScrollBars scrollBars;
public:
    UIScrollArea(UIWidget* parent);

    bool resizeEvent(UIResizeEvent event);
    bool paintEvent(UISurface *surface);

    UIScrollBars getScrollBars() { return scrollBars; }
    void setScrollBars(UIScrollBars bars);
    void scrollX(double value) { hScroll->addDoublePos(value); }
    void scrollY(double value) { vScroll->addDoublePos(value); }
    void scrollMouseMove(int v) { vScroll->mouseScroll(v); }
    UIScrollBox* getHScroll() { return hScroll; }
    UIScrollBox* getVScroll() { return vScroll; }
    UIWidget* getContainer() { return container; }
    UIWidget* getContent() { return content; }
};


#endif // UISCROLL_H
