
#define OSX
#define NO_SDL_GLEXT

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "common.h"
#include "Containers/containers.h"
#include "Strings/string.h"
#include "Strings/stringhash.h"
#include "Streams/streams.h"
#include "LuaWrapper/luaconfig.h"
#include "GL/glwindow.h"
#include "GL/UI/uitexture.h"
#include "GL/UI/uiskin.h"
#include "GL/UI/Widgets/uiframe.h"
#include "GL/UI/Widgets/uibutton.h"
#include "GL/UI/Widgets/uiboxlayout.h"
#include "GL/UI/Widgets/uigridlayout.h"
#include "GL/UI/Widgets/uiscroll.h"
#include "GL/UI/Widgets/uilistbox.h"




using namespace coon;


int main(/*int argc, char *argv[]*/)
{
    LuaConfig* config = new LuaConfig();
    GLWindow* window = new GLWindow(config);

    UIFrame* frame = new UIFrame(window->getRoot());
    frame->setName("Frame1");
    frame->setRect(Rect(10, 10, 620, 460));
    frame->setLayout(new UIHBoxLayout());
    ((UIBoxLayout*)frame->getLayout())->setBorder(30);
    window->getRoot()->getLayout()->addWidget(frame);

    /*UIListBox* list = new UIListBox(frame);
    list->setName("List1");
    frame->getLayout()->addWidget(list);

    for (int i = 0; i < 50; i++) {
        list->append("Item " + String::fromInt(i));
    }*/
    
    /*list->onNotify = [] (UIWidget* sender) {
        std::cout << ((UIListBox*) sender)->getSelIndex() << std::endl;
    };*/
    

    /*UIScrollArea* scroll = new UIScrollArea(frame);
    scroll->setName("Scroll1");
    frame->getLayout()->addWidget(scroll);*/

    /*UIScrollBox* scroll = new UIScrollBox(frame, UI_BOX_VERTICAL);
    scroll->setName("Scroll1");
    frame->getLayout()->addWidget(scroll);

    scroll = new UIScrollBox(frame, UI_BOX_HORIZONTAL);
    scroll->setName("Scroll2");
    frame->getLayout()->addWidget(scroll);*/

    /*scroll->onClick = [](UIWidget* sender) { std::cout << "Clicked!" << std::endl; };
    scroll->onDblClick = [](UIWidget* sender) { std::cout << "Double Clicked!" << std::endl; };
    scroll->onMouseDrag =
            [](UIWidget* sender, int x, int y) { std::cout << "Dragged! " << x << "," << y << std::endl; };*/

    UIButton* button = new UIButton(frame);
    button->setName("Button1");
    button->setCaption("Press me _please_!");
    button->setRect(Rect(150, 80, 200, 40));
    frame->getLayout()->addWidget(button);
    
    window->getRoot()->getSkin()->getFont()->addColor("r", RGBA(240, 0, 0, 200));
    window->getRoot()->getSkin()->getFont()->addColor("t", RGBA(40, 50, 130, 200));

    button = new UIButton(frame);
    button->setName("Button2");
    button->setCaption("Press &rme &t_please_!");
    button->setRect(Rect(150, 80, 200, 40));
    frame->getLayout()->addWidget(button);

    /*button = new UIButton(frame);
    button->setName("Button3");
    button->setCaption("Press me _please_!");
    button->setRect(Rect(150, 80, 200, 40));
    frame->getLayout()->addWidget(button);*/


    /*frame = new UIFrame(window->getRoot());
    frame->setName("Frame2");
    frame->setRect(Rect(30, 30, 400, 300));
    frame->setLayout(new UILayout());
    window->getRoot()->getLayout()->addWidget(frame);

    frame = new UIFrame(window->getRoot()->getLayout()->getWidget("Frame2"));
    frame->setName("ChildFrame");
    frame->setRect(Rect(15, 15, 80, 250));
    window->getRoot()->getLayout()->getWidget("Frame2")->getLayout()->addWidget(frame);

    window->getRoot()->getLayout()->moveForward("Frame1");*/

    while (window->isRunning()) {
        window->startFrame();
        window->processEvents();

        window->drawUI();

        window->endFrame();
    }

    delete window;
    delete config;

    return 0;
}
