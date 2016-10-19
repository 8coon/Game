
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
    
    
    UIFrame* frame = new UIFrame(window->getRoot());
    frame->setName("Frame1");
    frame->setRect(Rect(10, 10, 620, 460));
    frame->setLayout(new UIHBoxLayout());
    ((UIBoxLayout*)frame->getLayout())->setBorder(30);
    window->getRoot()->getLayout()->addWidget(frame);
    
    UIListBox* list = new UIListBox(frame);
    list->setName("List1");
    frame->getLayout()->addWidget(list);
    
    for (int i = 0; i < 50; i++) {
        list->append("Item " + String::fromInt(i));
    }
    
    
    SNCubeNode* cube = new SNCubeNode();
    cube->scaleX(0.5);
    cube->scaleY(0.5);
    cube->scaleZ(0.5);
    cube->rotateY(70);
    cube->moveX(0.83);
    
    
    //frame->setVisible(false);
    //list->setVisible(false);
    

    while (window->isRunning()) {
        window->startFrame();
        window->processEvents();
        
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
