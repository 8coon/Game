#include "uiwidget.h"
#include "uiboxlayout.h"



UIWidget::UIWidget(UIWidget *parent)
{
    this->parent = parent;
    if (parent != NULL) {
        this->skin = parent->skin;
    }
}


UIWidget::~UIWidget()
{
    if (layout != NULL) delete layout;
}


void UIRendererSurface::begin()
{
    SDL_SetRenderTarget(getRenderer(), this->texture);
    Point p = Point(getRect().x, getRect().y);
    UISurface* parent = getParent();

    while (parent != NULL) {
        p.x += parent->getRect().x;
        p.y += parent->getRect().y;
        parent = parent->getParent();
    }

    setOffset(Point(-1 * p.x, -1 * p.y));
}


void UIRendererSurface::end()
{
    SDL_SetRenderTarget(getRenderer(), NULL);
    setOffset(Point(0, 0));

    UITexture* texObj = new UITexture(this->texture);
    drawTexture(texObj, -getRect().x, -getRect().y);
    texObj->setTextureUnsafe(NULL);
    delete texObj;
}


void UIWindowSurface::drawTexture(UITexture *texture, Rect src, Rect dst)
{
    /*ltClip = Point(0, 0);
    rbClip = Point(65535, 65535);
    Stack<Rect> tempStack;

    while (!clippingRects.empty()) {
        Rect clip = clippingRects.front();
        if (clip.x > ltClip.x) ltClip.x = clip.x;
        if (clip.y > ltClip.y) ltClip.y = clip.y;
        if ((clip.x + clip.w) < rbClip.x) rbClip.x = clip.x + clip.w;
        if ((clip.y + clip.h) < rbClip.y) rbClip.y = clip.y + clip.h;
        tempStack.push(clip);
        clippingRects.pop();
    }

    while (!tempStack.empty()) {
        clippingRects.push(tempStack.top());
        tempStack.pop();
    }

    Point lt = Point(dst.x, dst.y);
    Point rb = Point(dst.x + dst.w, dst.y + dst.h);
    Point ltSrc = Point(src.x, src.y);
    Point rbSrc = Point(src.x + src.w, src.y + src.h);

    if (lt.x < ltClip.x) {
        ltSrc.x += ltClip.x - lt.x;
        lt.x = ltClip.x;
    } else if (lt.y < ltClip.y) {
        ltSrc.y += ltClip.y - lt.y;
        lt.y = ltClip.y;
    } else if (rb.x > rbClip.x) {
        rbSrc.x -= rb.x - rbClip.x;
        rb.x = rbClip.x;
    } else if (rb.y > rbClip.y) {
        rbSrc.y -= rb.y - rbClip.y;
        rb.y = rbClip.y;
    }

    Rect _src = Rect(ltSrc.x, ltSrc.y, rbSrc.x - ltSrc.x, rbSrc.y - ltSrc.y);
    Rect _dst = Rect(lt.x, lt.y, rb.x - lt.x, rb.y - lt.y);*/

    getWindow()->drawTexture(texture, src, dst);
}


void UISurface::drawTexture(UITexture *texture, Rect src, Rect dst)
{
    Rect _dst = Rect(rect.x + offset.x + dst.x, rect.y + offset.y + dst.y, dst.w, dst.h);
    getParent()->drawTexture(texture, src, _dst);
}


void UISurface::drawTexture(UITexture *texture, Rect src, int x, int y)
{
    drawTexture(texture, src, Rect(x, y, texture->getWidth(), texture->getHeight()));
}


void UISurface::drawTexture(UITexture *texture, int x, int y)
{
    drawTexture(texture, Rect(0, 0, texture->getWidth(), texture->getHeight()), x, y);
}


UISurface::UISurface(UISurface *parent, Rect rect)
{
    this->parent = parent;
    this->rect = rect;
}


void UISurface::drawWidget(UIWidgetTexture texture, int height, int width)
{
    UIWidgetTexture dstTexture(NULL, Rect(0, 0, texture.getLeft(), texture.getTop()),
                               Point(width, height));

    drawTexture(texture.getTexture(), texture.getRect(UI_TOPLEFT), dstTexture.getRect(UI_TOPLEFT));
    drawTexture(texture.getTexture(), texture.getRect(UI_TOP), dstTexture.getRect(UI_TOP));
    drawTexture(texture.getTexture(), texture.getRect(UI_TOPRIGHT), dstTexture.getRect(UI_TOPRIGHT));
    drawTexture(texture.getTexture(), texture.getRect(UI_RIGHT), dstTexture.getRect(UI_RIGHT));
    drawTexture(texture.getTexture(), texture.getRect(UI_BOTTOMRIGHT), dstTexture.getRect(UI_BOTTOMRIGHT));
    drawTexture(texture.getTexture(), texture.getRect(UI_BOTTOM), dstTexture.getRect(UI_BOTTOM));
    drawTexture(texture.getTexture(), texture.getRect(UI_BOTTOMLEFT), dstTexture.getRect(UI_BOTTOMLEFT));
    drawTexture(texture.getTexture(), texture.getRect(UI_LEFT), dstTexture.getRect(UI_LEFT));
    drawTexture(texture.getTexture(), texture.getRect(UI_CENTER), dstTexture.getRect(UI_CENTER));
}


void UISurface::drawLetter(UIFont* font, const String &letter, Rect dst, RGBA color)
{
    Rect src = font->getLetter(letter);

    SDL_SetTextureColorMod(font->getTexture()->getTexture(), color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(font->getTexture()->getTexture(), color.a);
    drawTexture(font->getTexture(), src, dst);
}


void UISurface::drawText(UIFont *font, const String &text, Rect rect)
{
    bool tag = false;
    StringBuilder final;
    Stack<RGBA> color;
    color.push(font->getColor());
    int x = rect.x;
    int y = rect.y;

    for (const Char& ch: text) {
        if (!tag) {
            if (ch.equals(String("&").at(0))) {
                tag = true;
            } else {
                Rect src = font->getLetter(String(ch));
                Rect dst = src;
                dst.x = x;
                dst.y = y;

                drawLetter(font, String(ch), dst, color.top());
                x += dst.w;
            }
        } else {
            tag = false;
            if (ch.equals(String("^").at(0))) {
                if (color.size() > 0) color.pop();
            } else if (font->hasColor(ch)) {
                color.push(font->getColor(ch));
            }
        }

        if (x > rect.x + rect.w) return;
    }
}


UIWindowSurface::UIWindowSurface(GLWindow *window):
        UISurface(NULL, Rect(0, 0, window->getResolution().x, window->getResolution().y))
{
    this->window = window;
}


SDL_Renderer* UIWindowSurface::getRenderer()
{
    return getWindow()->getRenderer();
}


UIRendererSurface::UIRendererSurface(UISurface *parent, Rect rect): UISurface(parent, rect)
{
    texture = SDL_CreateTexture(getRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
}


UIRendererSurface::~UIRendererSurface()
{
    SDL_DestroyTexture(texture);
}


void UIWidgetDeleter::operator()(UIWidget* ptr)
{
    if (!weak) delete ptr;
}


void UILayout::addWidget(UIWidget* widget)
{
    widgets.insert(Pair<int, Pointer<UIWidget>>(widget->getZOrder(),
                                                Pointer<UIWidget>(widget, UIWidgetDeleter())));
}


void UILayout::removeWidget(const String &name)
{
    UIWidget* widget = getWidget(name);
    Pair<OrderedMultiMap<int, Pointer<UIWidget>>::iterator, OrderedMultiMap<int, Pointer<UIWidget>>::iterator> p;
    p = widgets.equal_range(widget->getZOrder());

    for (OrderedMultiMap<int, Pointer<UIWidget>>::iterator i = p.first; i != p.second; ++i) {
        if ((*i).second->getName().equals(name)) {
            widgets.erase(i);
            return;
        }
    }
}


void UILayout::setZOrder(const String &name, int zOrder, bool relative)
{
    UIWidget* widget = getWidget(name);
    widget->setZOrder((relative)?(widget->getZOrder()):(0) + zOrder);

    Pair<OrderedMultiMap<int, Pointer<UIWidget>>::iterator, OrderedMultiMap<int, Pointer<UIWidget>>::iterator> p;
    p = widgets.equal_range(widget->getZOrder());

    for (OrderedMultiMap<int, Pointer<UIWidget>>::iterator i = p.first; i != p.second; ++i) {
        if ((*i).second->getName().equals(name)) {
            Pointer<UIWidget> pointer = (*i).second;
            widgets.insert(Pair<int, Pointer<UIWidget>>(widget->getZOrder(), pointer));
            widgets.erase(i);
            return;
        }
    }
}


void UILayout::moveToFront(const String &name)
{
    int min = INT32_MAX;
    for (auto& pair: widgets) {
        if (pair.first < min) min = pair.first;
    }

    setZOrder(name, min - 1);
}


void UILayout::sendToBack(const String &name)
{
    int max = INT32_MIN;
    for (auto& pair: widgets) {
        if (pair.first > max) max = pair.first;
    }

    setZOrder(name, max + 1);
}


void UILayout::moveForward(const String &name)
{
    setZOrder(name, -1, true);
}


void UILayout::sendBackward(const String &name)
{
    setZOrder(name, 1, true);
}


UIWidget* UILayout::getWidget(const String &name)
{
    for (auto& pair: widgets) {
        if (pair.second->getName().equals(name)) {
            return pair.second.get();
        }
    }

    return NULL;
}


Rect UIWidget::getPaddedRect()
{
    UIWidget* parent = this;//getParent();
    Rect rect = getRect();

    if (parent != NULL) {
        UIBoxLayout* layout = dynamic_cast<UIBoxLayout*>(parent->getLayout());

        if (layout != NULL) {
            rect.x += layout->getBorder();
            rect.y += layout->getBorder();
            rect.h -= layout->getBorder() * 2;
            rect.w -= layout->getBorder() * 2;
        }
    }

    return rect;
}


bool UIWidget::paintEvent(UISurface* surface)
{
    if (getLayout() != NULL) {
        for (auto& pair: *getLayout()) {
            if (pair.second->getVisible()) {
                UISurface childSurface = UISurface(surface, pair.second->getRect());
                UIResizeEvent event;
                event.height = pair.second->getHeight();
                event.width = pair.second->getWidth();
                pair.second->resizeEvent(event);
                pair.second->paintEvent(&childSurface);
            }
        }
    }

    return true;
}


bool UIWidget::mouseEvent(UIMouseEvent event)
{
    switch (event.type) {
        case UI_MOUSE_MOVED: {
            if (lastDownTime > lastUpTime) {   // mouse last was pressed down after it was last pressed up
                if (onMouseDrag != NULL) onMouseDrag(this, event.point.x, event.point.y);
            } else {
                if (onMouseMove != NULL) onMouseMove(this, event.point.x, event.point.y);
            }

            if (getState() != UI_PRESSED) setState(UI_HOVER);
        } break;
        case UI_MOUSE_PRESSED: {
            setState(UI_PRESSED);
            lastDownTime = getTime();
            if (onMouseDown != NULL) onMouseDown(this, event.point.x, event.point.y);
        } break;
        case UI_MOUSE_RELEASED: {
            lastUpTime = getTime();
            if (onMouseUp != NULL) onMouseUp(this, event.point.x, event.point.y);
            if (lastUpTime - lastDownTime < 100) {
                if (lastUpTime - lastClickTime < 300) {
                    if (onDblClick != NULL) onDblClick(this);
                } else {
                    if (onClick != NULL) onClick(this);
                    lastClickTime = lastUpTime;
                }
            }

            if (getRect().intersectsWith(event.point.x + getLeft(), event.point.y + getTop())) {
                setState(UI_HOVER);
            } else {
                setState(UI_NORMAL);
            }
        } break;
        case UI_MOUSE_SCROLL: {
            if (onMouseScroll != NULL) onMouseScroll(this, event.scroll);
        } break;
        case UI_MOUSE_OUT: {
            lastUpTime = getTime();
            setState(UI_NORMAL);
        } break;
    }

    if (getLayout() != NULL) {
        for (auto& pair: *getLayout()) {
            if (pair.second->getVisible()) {
                if (pair.second->getRect().intersectsWith(event.point.x, event.point.y)) {
                    if (pair.second->getEnabled()) {
                        UIMouseEvent ev = event;
                        ev.point.x -= pair.second->getLeft();
                        ev.point.y -= pair.second->getTop();

                        pair.second->mouseEvent(ev);
                    }
                } else {
                    UIMouseEvent ev = event;

                    if (pair.second->isMouseDown() && pair.second->getEnabled()
                            && ((event.type == UI_MOUSE_MOVED))) {
                        ev.point.x -= pair.second->getLeft();
                        ev.point.y -= pair.second->getTop();
                    } else {
                        ev.type = UI_MOUSE_OUT;
                    }

                    pair.second->mouseEvent(ev);
                }
            }
        }
    }

    return true;
}


void UIWidget::setHeight(int v)
{
    if (v > getMaxHeight()) v = getMaxHeight();
    if (v < getMinHeight()) v = getMinHeight();
    rect.h = v;
}


void UIWidget::setWidth(int v)
{
    if (v > getMaxWidth()) v = getMaxWidth();
    if (v < getMinWidth()) v = getMinWidth();
    rect.w = v;
}


void UIWidget::setRect(Rect rect)
{
    setHeight(rect.h);
    setWidth(rect.w);
    this->rect.x = rect.x;
    this->rect.y = rect.y;
}


bool UIAdjustiveLayout::resizeEvent(UIResizeEvent event)
{
    int dh = event.height - getContainer()->getHeight();
    int dw = event.width - getContainer()->getWidth();

    for (auto& pair: *this) {
        UIResizeEvent ev;
        ev.height = pair.second->getHeight() + dh;
        ev.width = pair.second->getWidth() + dw;
        pair.second->resizeEvent(ev);
    }

    return true;
}
