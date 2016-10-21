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

#include "../../SDL2/SDL.h"
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>


using namespace coon;


class Scene
{
private:
    Pointer<ISceneNode> root;
    RGBA ambientColor = RGBA(255, 255, 255, 0);
public:
    Scene() { root = Pointer<ISceneNode>(new ISceneNode("root")); }
    virtual ~Scene() {}
    
    virtual void draw(GLContext* context);
    
    ISceneNode* getRoot() { return root.get(); }
    ISceneNode* findNode(const String& name) { return root->findNodeByName(name); }
    
    RGBA getAmbientColor() { return ambientColor; }
    void setAmbientColor(const RGBA& color) { ambientColor = color; }
    
    void setLighting(bool val);
};

#endif /* scene_hpp */

















