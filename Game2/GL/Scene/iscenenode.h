#ifndef iscenenode_hpp
#define iscenenode_hpp

#include <stdio.h>
#include "../../Strings/stringhash.h"
#include "../../Containers/containers.h"
#include "../../Strings/string.h"
#include "../glwindow.h"
#include "../glcontext.h"
#include "../gltexture.h"

#include "../../SDL2/SDL.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


using namespace coon;


class SNLightNode;


class ISceneNode
{
private:
    Vector3df pos = Vector3df(0.0, 0.0, 0.0);
    Vector3df rot = Vector3df(0.0, 0.0, 0.0);
    Vector3df scale = Vector3df(1.0, 1.0, 1.0);
    Map<String, Pointer<ISceneNode>> childNodes;
    GLTexture* texture = NULL;
    String name;
    
    int index = 0;
protected:
    virtual void render(GLContext* context) {}
    virtual void renderChildren(GLContext* context);
public:
    ISceneNode(const String& name) { setName(name); }
    virtual ~ISceneNode() {}
    
    void draw(GLContext* context, bool alsoCamera = false);
    
    void setTexture(GLTexture* texture) { this->texture = texture; }
    GLTexture* getTexture() { return texture; }
    
    int getLightIndex() { return index; }
    void setLightIndex(int val) { index = val; }
    
    void addChild(const String& name, ISceneNode* node)
        { childNodes.insert(makePair(name, Pointer<ISceneNode>(node)));
            node->setName(name);}
    void addChild(ISceneNode* node) { addChild(node->getName(), node); }
    void removeChild(const String& name) { childNodes.erase(name); }
    void removeChildren() { childNodes.clear(); }
    bool hasChild(const String& name)
        { return childNodes.find(name) != childNodes.end(); }
    ISceneNode* getChild(const String& name) { return childNodes.at(name).get(); }
    ISceneNode* findNodeByName(const String& name);
    
    virtual bool isLightEmitter() { return false; }
    virtual bool isCamera() { return false; }
    
    void setPos(const Vector3df& pos) { this->pos = pos; }
    void setRot(const Vector3df& rot) { this->rot = rot; }
    void setScale(const Vector3df& scale) { this->scale = scale; }
    Vector3df& getPos() { return pos; }
    Vector3df& getRot() { return rot; }
    Vector3df& getScale() { return scale; }
    
    void rotateX(float val) { rot.x += val; }
    void rotateY(float val) { rot.y += val; }
    void rotateZ(float val) { rot.z += val; }
    float getXRot() { return rot.x; }
    float getYRot() { return rot.y; }
    float getZRot() { return rot.z; }
    
    void moveX(float val) { pos.x += val; }
    void moveY(float val) { pos.y += val; }
    void moveZ(float val) { pos.z += val; }
    float getXPos() { return pos.x; }
    float getYPos() { return pos.y; }
    float getZPos() { return pos.z; }
    
    void scaleX(float val) { if (val > 0) scale.x *= val; }
    void scaleY(float val) { if (val > 0) scale.y *= val; }
    void scaleZ(float val) { if (val > 0) scale.z *= val; }
    float getXScale() { return scale.x; }
    float getYScale() { return scale.y; }
    float getZScale() { return scale.z; }
    
    Map<String, Pointer<ISceneNode>>::iterator begin()
        { return childNodes.begin(); }
    Map<String, Pointer<ISceneNode>>::const_iterator begin() const
        { return childNodes.begin(); }
    Map<String, Pointer<ISceneNode>>::iterator end()
        { return childNodes.end(); }
    Map<String, Pointer<ISceneNode>>::const_iterator end() const
        { return childNodes.end(); }
    
    const String& getName() { return name; }
    void setName(const String& name) { this->name = name; }
};

#endif /* iscenenode_hpp */
