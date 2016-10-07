#include "uiscroll.h"


UIWidgetTexture UIScrollBoxButtonUp::getWidgetTexture()
{
    if (!getEnabled()) return getSkin()->getWidgetTexture(buildString("SCROLLBOX_UP_", "_GRAYED"));

    switch (getState()) {
        case UI_NORMAL: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_UP")); break;
        case UI_HOVER: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_UP_HOVER")); break;
        case UI_PRESSED: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_UP_PRESSED")); break;
        case UI_GRAYED: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_UP_GRAYED"));
    }

    return getSkin()->getWidgetTexture(buildString("SCROLLBOX_UP_", ""));
}


UIWidgetTexture UIScrollBoxButtonDown::getWidgetTexture()
{
    if (!getEnabled()) return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_DOWN_GRAYED"));

    switch (getState()) {
        case UI_NORMAL: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_DOWN")); break;
        case UI_HOVER: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_DOWN_HOVER")); break;
        case UI_PRESSED: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_DOWN_PRESSED")); break;
        case UI_GRAYED: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_DOWN_GRAYED"));
    }

    return getSkin()->getWidgetTexture(buildString("SCROLLBOX_DOWN_", ""));
}


UIWidgetTexture UIScrollBoxBar::getWidgetTexture()
{
    if (!getEnabled()) return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_GRAYED"));

    switch (getState()) {
        case UI_NORMAL: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "")); break;
        case UI_HOVER: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_HOVER")); break;
        case UI_PRESSED: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_PRESSED")); break;
        case UI_GRAYED: return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_GRAYED"));
    }

    return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", ""));
}


UIWidgetTexture UIScrollBox::getWidgetTexture()
{
    return getSkin()->getWidgetTexture(buildString("SCROLLBOX_", "_FRAME"));
}


UIScrollBox::UIScrollBox(UIWidget *parent, UIBoxOrientation ornt): UIButton(parent, NULL)
{
    this->ornt = ornt;
    setLayout(new UIBoxLayout(ornt));

    buttonUp = new UIScrollBoxButtonUp(this, ornt);
    getLayout()->addWidget(buttonUp);

    barContainer = new UIWidget(this);
    barContainer->setLayout(new UIBoxLayout(ornt));
    getLayout()->addWidget(barContainer);

    bar = new UIScrollBoxBar(this, ornt);
    barContainer->getLayout()->addWidget(bar);

    buttonDown = new UIScrollBoxButtonDown(this, ornt);
    getLayout()->addWidget(buttonDown);

    if (ornt == UI_BOX_HORIZONTAL) {
        barContainer->setMinHeight(buttonUp->getMinHeight());
        barContainer->setMaxHeight(barContainer->getMinHeight());
        bar->setMaxWidth(65535);
    } else {
        barContainer->setMinWidth(buttonUp->getMinWidth());
        barContainer->setMaxWidth(barContainer->getMinWidth());
        bar->setMaxHeight(65535);
    }


    bar->onMouseDown = [](UIWidget* sender, int x, int y) {
        UIScrollBox* parent = (UIScrollBox*)sender->getParent();
        parent->setLastMousePos(Point(x, y));
    };


    bar->onMouseDrag = [](UIWidget* sender, int x, int y) {
        UIScrollBox* parent = (UIScrollBox*)sender->getParent();
        Point last = parent->getLastMousePos();
        int dx = x - last.x;
        int dy = y - last.y;

        int ds = (parent->getOrientation() == UI_BOX_HORIZONTAL)?(dx):(dy);

        if (ds != 0) {
            double lds = (double)ds / parent->getPrice();
            parent->addDoublePos(lds);
        }
    };


    bar->onMouseScroll = [](UIWidget* sender, int v) {
        UIScrollBox* parent = (UIScrollBox*)sender->getParent();
        parent->setLastMousePos(Point(0, 0));
        sender->onMouseDrag(sender, v, v);
    };


    buttonUp->onClick = [](UIWidget* sender) {
        UIScrollBox* parent = (UIScrollBox*)sender->getParent();
        parent->addDoublePos(-1);
    };


    buttonDown->onClick = [](UIWidget* sender) {
        UIScrollBox* parent = (UIScrollBox*)sender->getParent();
        parent->addDoublePos(1);
    };
}


bool UIScrollBox::resizeEvent(UIResizeEvent event)
{
    if (ornt == UI_BOX_HORIZONTAL) {
        barContainer->setMinWidth(event.width - buttonUp->getMinWidth() - buttonDown->getMinWidth());
        barContainer->setMaxWidth(barContainer->getMinWidth());
        barContainer->setWidth(0);
        setMinHeight(buttonUp->getMinHeight());
        setMaxHeight(getMinHeight());
    } else {
        barContainer->setMinHeight(event.height - buttonUp->getMinHeight() - buttonDown->getMinHeight());
        barContainer->setMaxHeight(barContainer->getMinHeight());
        barContainer->setHeight(0);
        setMinWidth(buttonUp->getMinWidth());
        setMaxWidth(getMinWidth());
    }

    rearrange();
    return this->UIButton::resizeEvent(event);
}


void UIScrollBox::rearrange()
{
    double price = getPrice();
    int barContainerSize = getBarContainerSize();
    int barSize = price * barContainerSize * 0.01;
    if (barSize <= SCROLL_BAR_SIZE) barSize = SCROLL_BAR_SIZE;
    if (barSize > barContainerSize) barSize = barContainerSize;

    if (ornt == UI_BOX_HORIZONTAL) {
        bar->setMinWidth(barSize);
        bar->setMaxWidth(barSize);
        ((UIBoxLayout*)barContainer->getLayout())->setOffsetX(getDoublePos() * price);
    } else {
        bar->setMinHeight(barSize);
        bar->setMaxHeight(barSize);
        ((UIBoxLayout*)barContainer->getLayout())->setOffsetY(getDoublePos() * price);
    }
}


int UIScrollBox::getBarContainerSize()
{
    int barContainerSize = ((ornt == UI_BOX_HORIZONTAL)?(getWidth()):(getHeight()));
    barContainerSize -= SCROLL_BUTTON_SIZE * 2;
    return barContainerSize;
}


double UIScrollBox::getPrice()
{
    int barContainerSize = getBarContainerSize();
    int barSize = (ornt == UI_BOX_HORIZONTAL)?(bar->getWidth()):(bar->getHeight());
    double price = (double)(barContainerSize - barSize) / (getMax() - getMin());
    return price;
}



UIScrollArea::UIScrollArea(UIWidget *parent): UIWidget(parent)
{
    setLayout(new UIGridLayout(2, 2));

    container = new UIWidget(this);
    container->setLayout(new UIHBoxLayout());
    content = new UIWidget(container);
    content->setLayout(new UIVBoxLayout());
    container->getLayout()->addWidget(content);
    getLayout()->addWidget(container);

    vScroll = new UIScrollBox(this, UI_BOX_VERTICAL);
    vScroll->setTag(this);
    getLayout()->addWidget(vScroll);

    hScroll = new UIScrollBox(this, UI_BOX_HORIZONTAL);
    hScroll->setTag(this);
    getLayout()->addWidget(hScroll);

    container->onMouseScroll = [](UIWidget* sender, int v) {
        UIScrollArea* parent = (UIScrollArea*)sender->getParent();
        parent->scrollMouseMove(v);
    };

    vScroll->onChange = [](UIWidget* sender) {
        UIScrollBox* box = (UIScrollBox*)sender;
        UIScrollArea* parent = (UIScrollArea*)box->getTag();
        UIWidget* container = parent->getContainer();
        ((UIBoxLayout*)(container->getLayout()))->setOffsetY(-1 * box->getPos());
        if (parent->onChange != NULL) parent->onChange(parent);
    };

    hScroll->onChange = [](UIWidget* sender) {
        UIScrollBox* box = (UIScrollBox*)sender;
        UIScrollArea* parent = (UIScrollArea*)box->getTag();
        UIWidget* container = parent->getContainer();
        ((UIBoxLayout*)(container->getLayout()))->setOffsetX(-1 * box->getPos());
    };
}


bool UIScrollArea::resizeEvent(UIResizeEvent event)
{
    container->setMinWidth(event.width - vScroll->getWidth());
    container->setMinHeight(event.height - vScroll->getWidth());
    container->setMaxWidth(container->getMinWidth());
    container->setMaxHeight(container->getMinHeight());
    container->setWidth(container->getMinWidth());
    container->setHeight(container->getMinHeight());

    hScroll->setMinWidth(container->getMinWidth());
    vScroll->setMinHeight(container->getMinHeight());

    int maxHeight = -1 * container->getHeight();
    int maxWidth = -1 * container->getWidth();

    for (auto& pair: *(content->getLayout())) {
        maxWidth += pair.second->getWidth();
        maxHeight += pair.second->getHeight();
    }

    hScroll->setMax(maxWidth);
    vScroll->setMax(maxHeight);

    return this->UIWidget::resizeEvent(event);
}


bool UIScrollArea::paintEvent(UISurface *surface)
{
    UIRendererSurface* renderer = new UIRendererSurface(surface, surface->getRect());

    renderer->begin();
    UIWidgetTexture texture = getWidgetTexture();
    renderer->drawWidget(texture, getHeight(), getWidth());
    bool result = this->UIWidget::paintEvent(renderer);
    renderer->end();

    delete renderer;
    return result;
}


void UIScrollArea::setScrollBars(UIScrollBars bars)
{
    scrollBars = bars;

    switch (bars) {
        case UI_HORIZONTAL: hScroll->setVisible(true); vScroll->setVisible(false); break;
        case UI_VERTICAL: hScroll->setVisible(false); vScroll->setVisible(true); break;
        case UI_BOTH: hScroll->setVisible(true); vScroll->setVisible(true); break;
        case UI_NONE: hScroll->setVisible(false); vScroll->setVisible(false); break;
    }
}
