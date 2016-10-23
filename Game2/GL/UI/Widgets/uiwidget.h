#ifndef UIWIDGET_H
#define UIWIDGET_H

#include "../../../common.h"
#include "../../../Containers/containers.h"
#include "../uiskin.h"
#include "../uiwidgettexture.h"
#include "../../glwindow.h"

#include <memory>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>



class UILayout;


enum UIWidgetState { UI_NORMAL, UI_HOVER, UI_PRESSED, UI_GRAYED };


class GLWindow;


class UISurface
{
private:
    UISurface* parent;
    Rect rect;
    Point offset;
protected:
    virtual void drawLetter(UIFont* font, const String &letter, Rect dst, RGBA color);
public:
    UISurface(UISurface* parent, Rect rect);

    UISurface* getParent() { return parent; }
    Rect getRect() { return rect; }
    void setOffset(Point offset) { this->offset = offset; }
    Point getOffset() { return offset; }

    virtual SDL_Renderer* getRenderer() { if (getParent() != NULL) return getParent()->getRenderer(); return NULL; }
    virtual void drawTexture(UITexture* texture, Rect src, Rect dst,
                             GLuint gltex = 0);
    virtual void drawTexture(UITexture* texture, Rect src, int x, int y,
                             GLuint gltex = 0);
    virtual void drawTexture(UITexture* texture, int x, int y, GLuint gltex = 0);
    virtual void drawWidget(UIWidgetTexture texture, int height, int width);
    virtual void drawText(UIFont* font, const String& text, Rect rect);
    virtual GLWindow* getWindow()
        { if (parent != NULL) return parent->getWindow(); return NULL; }
};


class UIWindowSurface: public UISurface
{
private:
    GLWindow* window;
    Queue<Rect> clippingRects;
    Point ltClip;
    Point rbClip;
public:
    UIWindowSurface(GLWindow* window);
    GLWindow* getWindow() { return window; }
    SDL_Renderer* getRenderer();

    void drawTexture(UITexture* texture, Rect src, Rect dst,
                     GLuint gltex = 0);
    void clip(Rect dst);
    void unClip() { clippingRects.pop(); }
    void clearClipping() { clippingRects = Queue<Rect>(); }
};


class UIRendererSurface: public UISurface
{
private:
    //SDL_Texture* texture;
    //SDL_Texture* prevTexture;
    
    GLuint frameBuffer;
    GLuint prevFrameBuffer;
    GLuint texture;
    GLuint renderBuffer;
public:
    UIRendererSurface(UISurface* parent, Rect rect);
    virtual ~UIRendererSurface();

    void begin();
    void end();
    
    GLuint getTexture() { return texture; }
    //UIWindowSurface* getWindowParent() { return (UIWindowSurface*) getParent(); }
};


enum UIMouseEventType { UI_MOUSE_PRESSED, UI_MOUSE_RELEASED, UI_MOUSE_MOVED, UI_MOUSE_SCROLL,
                      UI_MOUSE_OUT};
enum UIKeyEventType { UI_KEY_PRESSED, UI_KEY_RELEASED };
enum UIMouseButton { UI_BUTTON_NONE, UI_BUTTON_LEFT, UI_BUTTON_RIGHT, UI_BUTTON_MIDDLE };


struct UIMouseEvent
{
    Point point;
    Point global;
    int scroll = 0;
    int button = UI_BUTTON_NONE;
    UIMouseEventType type = UI_MOUSE_MOVED;

    UIMouseEvent(Point point, Point global) { this->point = point; this->global = global; }
    UIMouseEvent(UIMouseEvent& ev) {
        point = ev.point; global = ev.global; scroll = ev.scroll; button = ev.button; type = ev.type; }
    UIMouseEvent() {}
};


struct UIKeyEvent
{
    Char key;
    UIKeyEventType type;
};


struct UIResizeEvent
{
    int width;
    int height;
    UIResizeEvent(int width = 0, int height = 0) { this->width = width; this->height = height; }
};


class UISkin;
class UIWidget;


struct UIWidgetDeleter
{
    void operator()(UIWidget* ptr);
    bool weak = false;
};


class UILayout
{
private:
    UIWidget* container;
protected:
    OrderedMultiMap<int, Pointer<UIWidget>> widgets;
public:
    virtual ~UILayout() {}

    virtual void addWidget(UIWidget* widget);
    virtual void removeWidget(const String& name);
    virtual UIWidget* getWidget(const String& name);

    void setZOrder(const String& name, int zOrder, bool relative = false);
    void moveToFront(const String& name);
    void sendToBack(const String& name);
    void moveForward(const String& name);
    void sendBackward(const String& name);
    void clear(bool weak = false)
        { for (auto w: widgets) (std::get_deleter<UIWidgetDeleter>(w.second))->weak = weak; widgets.clear(); }

    UIWidget* getContainer() { return container; }
    void setContainer(UIWidget* container) { this->container = container; }

    virtual bool resizeEvent(UIResizeEvent /*event*/) { return true; }

    OrderedMultiMap<int, Pointer<UIWidget>>::iterator begin() { return widgets.begin(); }
    OrderedMultiMap<int, Pointer<UIWidget>>::const_iterator begin() const { return widgets.begin(); }
    OrderedMultiMap<int, Pointer<UIWidget>>::iterator end() { return widgets.end(); }
    OrderedMultiMap<int, Pointer<UIWidget>>::const_iterator end() const { return widgets.end(); }
};


class UIAdjustiveLayout: public UILayout
{
public:
    bool resizeEvent(UIResizeEvent event);
};


class UIWidget
{
private:
    UIWidget* parent;
    UILayout* layout = NULL;
    int zOrder = 0;
    String name;
    UISkin* skin;
    Rect rect = Rect(100, 300, 80, 40);
    UIWidgetState state = UI_NORMAL;
    bool enabled = true;
    bool visible = true;
    int minHeight = 3;
    int minWidth = 3;
    int maxHeight = 65535;
    int maxWidth = 65535;
    int lastDownTime = 0;
    int lastUpTime = 0;
    int lastClickTime = 0;
    void* tag = NULL;
public:
    UIWidget(UIWidget* parent = NULL);
    virtual ~UIWidget();

    void(*onClick)(UIWidget*) = [](UIWidget*) {};
    void(*onDblClick)(UIWidget*) = [](UIWidget*) {};
    void(*onMouseMove)(UIWidget*, int, int) = [](UIWidget*, int, int) {};
    void(*onMouseDown)(UIWidget*, int, int) = [](UIWidget*, int, int) {};
    void(*onMouseUp)(UIWidget*, int, int) = [](UIWidget*, int, int) {};
    void(*onMouseDrag)(UIWidget*, int, int) = [](UIWidget*, int, int) {};
    void(*onMouseScroll)(UIWidget*, int) = [](UIWidget*, int) {};
    void(*onPaint)(UIWidget*, UISurface*, Rect) = [](UIWidget*, UISurface*, Rect) {};  /* Not Called! */
    void(*onKeyPress)(UIWidget*, int) = [](UIWidget*, int) {};  /* Not Called! */
    void(*onKeyRelease)(UIWidget*, int) = [](UIWidget*, int) {};  /* Not Called! */
    void(*onChange)(UIWidget*) = [](UIWidget*) {};
    void(*onNotify)(UIWidget*) = [](UIWidget*) {};

    virtual bool onStateChange(UIWidgetState /*state*/) { return true; }
    virtual bool notify() { if (onNotify != NULL) onNotify(this); return true; }
    virtual bool paintEvent(UISurface* surface);
    virtual bool mouseEvent(UIMouseEvent event);
    virtual bool keyEvent(UIKeyEvent /*event*/) { return true; }
    virtual bool resizeEvent(UIResizeEvent event) {
        (hasLayout())?(getLayout()->resizeEvent(event)):(true);
        setHeight(event.height);
        setWidth(event.width);
        return true; }

    virtual UIWidgetTexture getWidgetTexture() { return UIWidgetTexture(); }

    Rect getRect() { return rect; }
    Rect getPaddedRect();
    void setRect(Rect rect);
    int getLeft() { return rect.x; }
    int getTop() { return rect.y; }
    int getHeight() { return rect.h; }
    int getWidth() { return rect.w; }
    void setLeft(int v) { rect.x = v; }
    void setTop(int v) { rect.y = v; }
    void setHeight(int v);
    void setWidth(int v);
    int getMinHeight() { return minHeight; }
    int getMinWidth() { return minWidth; }
    int getMaxHeight() { return maxHeight; }
    int getMaxWidth() { return maxWidth; }
    void setMinHeight(int v) { minHeight = v; }
    void setMinWidth(int v) { minWidth = v; }
    void setMaxHeight(int v) { maxHeight = v; }
    void setMaxWidth(int v) { maxWidth = v; }
    void setMinSize(int h, int w) { setMinHeight(h); setMinWidth(w); }
    void setMaxSize(int h, int w) { setMaxHeight(h); setMaxWidth(w); }

    int getZOrder() const { return zOrder; }
    void setZOrder(int zOrder) { this->zOrder = zOrder; }
    const String& getName() const { return name; }
    void setName(const String& name) { this->name = name; }
    UILayout* getLayout() { return layout; }
    void setLayout(UILayout* layout) { this->layout = layout; layout->setContainer(this); }
    UISkin* getSkin() { return skin; }
    void setSkin(UISkin* skin) { this->skin = skin; }
    UIWidgetState getState() { return state; }
    void setState(UIWidgetState state) { if (onStateChange(state)) this->state = state; }
    bool getEnabled() { return enabled; }
    void setEnabled(bool enabled) { this->enabled = enabled; }
    bool hasLayout() { return getLayout() != NULL; }
    UIWidget* getParent() { return parent; }
    bool isMouseDown() { return lastDownTime > lastUpTime; }
    bool getVisible() { return visible; }
    void setVisible(bool visible) { this->visible = visible; }
    void* getTag() { return tag; }
    void setTag(void* tag) { this->tag = tag; }
    bool isTagNull() { return getTag() == NULL; }
};




#endif // UIWIDGET_H
