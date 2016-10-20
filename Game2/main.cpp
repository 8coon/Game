
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
#include "GL/glcontext.h"
#include "GL/Scene/sncubenode.h"

#include <time.h>




using namespace coon;


int main()
{
    LuaConfig* config = new LuaConfig();
    GLWindow* window = new GLWindow(config);
    
    
    window->getSkin()->getFont()->addColor("r", RGBA(240, 0, 0, 200));
    window->getSkin()->getFont()->addColor("g", RGBA(0, 240, 0, 200));
    window->getSkin()->getFont()->addColor("b", RGBA(0, 0, 240, 200));
    window->getSkin()->getFont()->setColor(RGBA(255, 255, 255, 200));
    
    
    UIFrame* frame = new UIFrame(window->getRoot());
    frame->setName("Frame1");
    frame->setRect(Rect(10, 10, 620, 460));
    frame->setLayout(new UIHBoxLayout());
    ((UIBoxLayout*)frame->getLayout())->setBorder(30);
    window->getRoot()->getLayout()->addWidget(frame);
    
    
    UIButton* button = new UIButton(frame);
    button->setName("Button1");
    button->setCaption(
            "&rThe &gQuick &bBrown &rЛис");
    frame->getLayout()->addWidget(button);
    
    button = new UIButton(frame);
    button->setName("Button2");
    button->setVisible(false);
    frame->getLayout()->addWidget(button);

    
    /*UIListBox* list = new UIListBox(frame);
    list->setName("List1");
    frame->getLayout()->addWidget(list);
    
    for (int i = 0; i < 50; i++) {
        list->append("Item " + String::fromInt(i));
    }*/
    
    
    
    
    SNCubeNode* cube = new SNCubeNode();
    cube->moveZ(-3.9f);
    cube->moveX(0.9f);
    cube->rotateX(20.0f);
    
    UITexture* cubeTexture = new UITexture(
            window->getRenderer(), "Art/park копия.jpg");
    
    cube->setTexture(cubeTexture);
    
    //frame->setVisible(false);
    //list->setVisible(false);
    

    while (window->isRunning()) {
        window->startFrame();
        window->processEvents();
        
        cube->rotateY(1.0);
        cube->draw(window->getContext());
        
        window->drawUI();
        window->endFrame();
    }
    
    delete cube;

    delete window;
    delete config;
    
    SDL_Quit();
    return 0;
}
