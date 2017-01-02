

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
#include "GL/glshader.h"
#include "Game/2D/2dmap.h"
#include "Game/2D/2dgame.h"


#include <time.h>
#include <cmath>




using namespace coon;


int main()
{
    C2DGame game("Scripts/Game/main.lua");
    
    SDL_Quit();
    return 0;
}


int main2()
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
    frame->getLayout()->addWidget(button);
    
    button = new UIButton(frame);
    button->setName("Button2");
    button->setVisible(false);
    frame->getLayout()->addWidget(button);
    
    
    UIListBox* list = new UIListBox(frame);
    list->setName("List1");
    frame->getLayout()->addWidget(list);
    
    for (int i = 0; i < 10; i++) {
        list->append("Item " + String::fromInt(i));
    }
    
    //frame->setVisible(false);
    //list->setVisible(false);
    
    
    //GLTexture* cubeTexture = new GLTexture(window->getRenderer(),
    //                                       "Art/park копия.jpg");
    
    GLTexture* grassTexture = new GLTexture(window->getRenderer(), "Art/grass2.jpg");
    grassTexture->generateMipmaps();
    
    GLTexture* sandTexture = new GLTexture(window->getRenderer(), "Art/sand.jpg");
    sandTexture->generateMipmaps();
    
    GLTexture* rockTexture = new GLTexture(window->getRenderer(), "Art/rock.jpg");
    rockTexture->generateMipmaps();
    
    GLTexture* colorTexture = new GLTexture(window->getRenderer(),
                                             "Art/texcolor.png");
    GLTexture* heightTexture = new GLTexture(window->getRenderer(),
                                             "Art/heightmap_min.png");
    
    
    ISceneNode* holder = new ISceneNode("holder");
    //holder->moveZ(-20.0f);
    //holder->moveY(-30.0f);
    //holder->moveY(-0.5f);
    //holder->rotateX(10.0f);
    //holder->setScale(Vector3df(0.01f, 1.0f, 0.01f));
    scene->getScene()->getRoot()->addChild(holder);
    
    
    MSTexHeightmap* heightmap = new MSTexHeightmap(heightTexture);
    heightmap->setTexScale(50.0f);
    SNLandscapeNode* landscape = new SNLandscapeNode("land", heightmap);
    landscape->setScale(Vector3df(0.25f, /*6.25f*/12.5f, 0.25f));
    
    landscape->setColorMap(colorTexture);
    landscape->setTextureG(grassTexture);
    landscape->setTextureB(sandTexture);
    landscape->setTextureR(rockTexture);
    
    holder->addChild(landscape);
    
    
    /*ISceneNode* cameraHolder = new SNCubeNode("cameraHolder");
    //cameraHolder->moveY(40.0f);
    scene->getScene()->getRoot()->addChild(cameraHolder);
    
    ISceneNode* cameraRotater = new SNCubeNode("cameraRotater");
    cameraHolder->addChild(cameraRotater);*/
    
    SNCubeNode* cube = new SNCubeNode("cube");
    cube->scaleY(5.0f);
    //landscape->addChild(cube);
    
    
    SNCameraNode* camera = new SNCameraNode("camera");
    //camera->moveY(-40.0f);
    //cameraRotater->moveY(10.0f);
    holder->addChild(camera);
    scene->getScene()->setActiveCamera(camera);
    
    
    
    UIActionHandler moveHandler = [](const UIAction* actionData) {
        static int prevX = actionData->data.coord.x;
        static int prevY = actionData->data.coord.y;
        int xDelta = actionData->data.coord.x - prevX;
        int yDelta = actionData->data.coord.y - prevY;
        
        Scene* scene = actionData->scene->getScene();
        ISceneNode* land = scene->findNode("land");
        
        if (actionData->type == UIA_MOUSEDRAG) {
            if (actionData->binder->isMouseDown(UIBUTTON_LEFT)) {
                float angle = scene->getActiveCamera()->getYRot();
                angle *= 0.0174533;
                
                float sinA = sin(angle);
                float cosA = cos(angle);
                float x = xDelta * 0.1f;
                float y = yDelta * 0.1f;
                
                land->moveZ(sinA * x + cosA * y);
                land->moveX(cosA * x - sinA * y);
            } else if (actionData->binder->isMouseDown(UIBUTTON_RIGHT)) {
                scene->getActiveCamera()->rotateY(xDelta * 0.1f);
                scene->getActiveCamera()->rotateX(yDelta * 0.1f);
            }
        
        }
        
        prevX = actionData->data.coord.x;
        prevY = actionData->data.coord.y;
    };
    
    UIActionHandler scrollHandler = [](const UIAction* actionData) {
        Scene* scene = actionData->scene->getScene();
        ISceneNode* land = scene->findNode("land");
        land->moveY(actionData->data.coord.y * 0.5f);
    };
    
    UIActionHandler mouseMoveHandler = [](const UIAction* actionData) {
        Scene* scene = actionData->scene->getScene();
        SNLandscapeNode* land = (SNLandscapeNode*) scene->findNode("land");
        
        float sizeX = land->getLandscape()->getSizeX();
        float sizeY = land->getLandscape()->getSizeY();
        float x = actionData->data.coord.x;
        float y = actionData->data.coord.y;
        float angle = scene->getActiveCamera()->getYRot();
        angle *= 0.0174533;
        
        float sinA = sin(angle);
        float cosA = cos(angle);
        
        x /= actionData->scene->getWidth();
        y /= actionData->scene->getHeight();
        x *= 2.0f;
        y *= -2.0f;
        x += 1.0f;
        y += 1.0f;
    
        //x *= 0.25f * 0.5f;
        //y *= 0.25f * 0.5f;
        
        //x = y * sinA + x * cosA;
        //y = y * cosA - x * sinA;
        //x *= -1.0f;
        //y *= -1.0f;
        
        //std::cout << x << ", " << y << std::endl;
        
        //x -= 0.5f;
        //y -= 0.5f;
        
        //x -= 2 * x;
        //y -= 2 * y;
        //x *= -1.0f;
        //y *= -1.0f;
        
        GLContext* context = actionData->scene->getContext();
        
        context->pushMatrix(GLM_BOTH, true);
        scene->getActiveCamera()->pushTransformations(context, 1.0f);
        
        Vector3df vec = scene->getActiveCamera()->rayCast(x, y);
        
        scene->getActiveCamera()->popTransformations(context);
        context->popMatrix();
        
       // ISceneNode* cube = scene->findNode("cube");
        
        //vec.x -= 2;
        //vec.z -= 2;
        //vec.x *= -1.0f;
        
        //vec.x = vec.x * cosA - vec.z * sinA;
        //vec.z = vec.x * sinA + vec.z * cosA;
        
        float len = vec.length();
        vec.x /= len;
        vec.z /= len;
        
        vec.x *= land->getYPos();
        vec.z *= land->getYPos();
        
        //std::cout << scene->getActiveCamera()->getYPos() << std::endl;
        
        //vec = Vector3df(-vec.x * sizeX, 1.0f, vec.z * sizeY);
        vec.x -= land->getXPos();
        vec.z -= land->getZPos();
        
       // cube->setPos(vec);
        
        //std::cout << vec << std::endl;
    };
    
    scene->getActionBinder()->bindAction(UIA_MOUSEDRAG, moveHandler);
    scene->getActionBinder()->bindAction(UIA_MOUSEDOWN, moveHandler);
    scene->getActionBinder()->bindAction(UIA_MOUSESCROLL, scrollHandler);
    scene->getActionBinder()->bindAction(UIA_MOUSEMOVE, mouseMoveHandler);
    
    
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
        //scene->getScene()->findNode("land")->rotateY(-0.05f);
        
        
        window->drawUI();
        window->endFrame();
    }

    delete window;
    delete config;
    
    SDL_Quit();
    return 0;
}
