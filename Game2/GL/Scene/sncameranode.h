#ifndef sncameranode_h
#define sncameranode_h

#include <stdio.h>
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


class SNCameraNode: public ISceneNode
{
protected:
    Vector3df lookAt = Vector3df(0.0, 0.0, 1.0);
    ISceneNode* rootNode = NULL;
    void render(GLContext* context);
public:
    SNCameraNode(const String& name): ISceneNode(name) {}
    
    void drawSceneRoot(GLContext* context, ISceneNode* root);
    
    bool isCamera() { return true; }
    
    void setLookAt(const Vector3df& lookAt) { this->lookAt = lookAt; }
    Vector3df getLookAt() { return lookAt; }
};


#endif /* sncameranode_hpp */
