
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
#include "GL/Scene/sncameranode.h"
#include "GL/Scene/scene.h"
#include "GL/Scene/snlandscapenode.h"


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
    
    
    GLTexture* cubeTexture = new GLTexture(window->getRenderer(),
                                           "Art/park копия.jpg");
    GLTexture* grassTexture = new GLTexture(window->getRenderer(),
                                            "Art/grass.jpg");
    grassTexture->setMipmapsEnabled(true);
    
    GLTexture* heightTexture = new GLTexture(window->getRenderer(),
                                             "Art/heightmap_min.png");
    
    
    ISceneNode* holder = new ISceneNode("holder");
    holder->moveZ(-3.9f);
    holder->moveY(-1.0f);
    holder->rotateX(10.0f);
    scene->getScene()->getRoot()->addChild(holder);
    
    
    //MSPlane* plane = new MSPlane(128, 128);
    MSTexHeightmap* heightmap = new MSTexHeightmap(heightTexture);
    heightmap->setTexScale(0.05f);
    SNLandscapeNode* landscape = new SNLandscapeNode("land", heightmap);
    landscape->setTexture(grassTexture);
    landscape->setScale(Vector3df(0.05f, 2.0f, 0.05f));
    holder->addChild(landscape);
    
    
    //SNLightNode* light = new SNLightNode("light");
    //cube->addChild(light);
    
    
    //float t = 0;

    while (window->isRunning()) {
        window->startFrame();
        window->processEvents();
        
        //scene->getScene()->findNode("cube")->rotateY(1.0f);
        /*scene->getScene()->findNode("lightCube")->setPos(
                Vector3df(1.5 * sin(t), light->getYPos(), 1.5 * cos(t)));
        scene->getScene()->findNode("lightCube")->rotateY(-18.0f);
        t -= 0.05;*/
        scene->getScene()->findNode("land")->rotateY(-0.05f);
        
        
        window->drawUI();
        window->endFrame();
    }

    delete window;
    delete config;
    
    SDL_Quit();
    return 0;
}
