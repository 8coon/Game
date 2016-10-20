#ifndef SNCubeNode_hpp
#define SNCubeNode_hpp

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


class SNCubeNode: public ISceneNode
{
protected:
    void render(GLContext* context);
public:
    SNCubeNode(const String& name): ISceneNode(name) {}
};


#endif /* SNCubeNode_hpp */
