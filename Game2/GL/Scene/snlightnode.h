#ifndef snlightnode_hpp
#define snlightnode_hpp

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


class SNLightNode: public ISceneNode
{
private:
    RGBA diffuse = RGBA(255, 255, 255, 255);
    RGBA specular = RGBA(255, 255, 255, 255);
    //RGBA emission = RGBA(0, 0, 0, 0);
    bool enabled = true;
    Vector3df direction = Vector3df(0.0, 0.0, -1.0);
protected:
    void render(GLContext* context);
public:
    SNLightNode(const String& name): ISceneNode(name) {}
    virtual ~SNLightNode() {}
    
    bool isEnabled() { return enabled; }
    void setEnabled(bool enabled) { this->enabled = enabled; }
    
    RGBA getDiffuse() { return diffuse; }
    RGBA getSpecular() { return specular; }
    void setDiffuse(const RGBA& color) { diffuse = color; }
    void setSpecular(const RGBA& color) { specular = color; }
    Vector3df getDirection() { return direction; }
    void setDirection(const Vector3df& vec) { direction = vec; }
};


#endif /* snlightnode_hpp */
