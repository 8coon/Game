#ifndef scene_hpp
#define scene_hpp

#include <stdio.h>
#include "../../common.h"
#include "../../Strings/stringhash.h"
#include "../../Containers/containers.h"
#include "../../Strings/string.h"
#include "../glwindow.h"
#include "../glcontext.h"
#include "iscenenode.h"
#include "sncameranode.h"

#include "../../SDL2/SDL.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


using namespace coon;


class Scene
{
private:
    Pointer<ISceneNode> root;
    SNCameraNode* activeCamera;
    RGBA ambientColor = RGBA(255, 255, 255, 0);
public:
    Scene();
    virtual ~Scene() {}
    
    virtual void draw(GLContext* context);
    
    ISceneNode* getRoot() { return root.get(); }
    ISceneNode* findNode(const String& name) { return root->findNodeByName(name); }
    
    RGBA getAmbientColor() { return ambientColor; }
    void setAmbientColor(const RGBA& color) { ambientColor = color; }
    
    void setLighting(bool val);
    
    void setActiveCamera(SNCameraNode* camera) { activeCamera = camera; }
    SNCameraNode* getActiveCamera() { return activeCamera; }
};

#endif /* scene_hpp */

















