
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
#include "GL/UI/Widgets/uiscene.h"
#include "GL/glcontext.h"
#include "GL/gltexture.h"
#include "GL/Scene/sncubenode.h"
#include "GL/Scene/snlightnode.h"
#include "GL/Scene/scene.h"


#include <time.h>
#include <cmath>




using namespace coon;


int main()
{
    LuaConfig* config = new LuaConfig();
    GLWindow* window = new GLWindow(config);
    
    
    window->getSkin()->getFont()->addColor("r", RGBA(240, 0, 0, 200));
    window->getSkin()->getFont()->addColor("g", RGBA(0, 240, 0, 200));
    window->getSkin()->getFont()->addColor("b", RGBA(0, 0, 240, 200));
    window->getSkin()->getFont()->setColor(RGBA(255, 255, 255, 200));
    
    
    UIScene* scene = new UIScene(window->getRoot());
    scene->setContext(window->getContext());
    window->getRoot()->getLayout()->addWidget(scene);
    
    
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
    button->setVisible(false);
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
    frame->setVisible(false);
    //list->setVisible(false);
    
    
    ISceneNode* holder = new ISceneNode("holder");
    holder->moveZ(-3.9f);
    holder->rotateX(20.0f);
    scene->getScene()->getRoot()->addChild(holder);
    
    SNCubeNode* cube = new SNCubeNode("cube");
    GLTexture* cubeTexture = new GLTexture(
            window->getRenderer(), "Art/park копия.jpg");
    cube->setTexture(cubeTexture);
    holder->addChild(cube);
    
    cube = new SNCubeNode("lightCube");
    cube->setScale(Vector3df(0.2, 0.2, 0.2));
    holder->addChild(cube);
    
    SNLightNode* light = new SNLightNode("light");
    cube->addChild(light);
    
    
    scene->getScene()->setLighting(true);
    scene->getScene()->setAmbientColor(RGBA(150, 150, 150, 100));
    
    float t = 0;

    while (window->isRunning()) {
        window->startFrame();
        window->processEvents();
        
        scene->getScene()->findNode("cube")->rotateY(1.0f);
        scene->getScene()->findNode("lightCube")->setPos(
                Vector3df(1.5 * sin(t), light->getYPos(), 1.5 * cos(t)));
        scene->getScene()->findNode("lightCube")->rotateY(-18.0f);
        t -= 0.05;
        
        window->drawUI();
        window->endFrame();
    }

    delete window;
    delete config;
    
    SDL_Quit();
    return 0;
}
