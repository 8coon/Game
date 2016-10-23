#ifndef sclandscapenode_hpp
#define sclandscapenode_hpp

#include <stdio.h>
#include "../../Strings/stringhash.h"
#include "../../Containers/containers.h"
#include "../../Strings/string.h"
#include "../glwindow.h"
#include "../glcontext.h"
#include "iscenenode.h"

#include "../../SDL2/SDL.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


class MSLandscape
{
private:
    float texScale = 1.0;
public:
    virtual ~MSLandscape() {}
    virtual float getHeightAt(const int x, const int y) = 0;
    virtual int getSizeX() = 0;
    virtual int getSizeY() = 0;
    float getTexScale() { return texScale; }
    void setTexScale(float scale) { texScale = scale; }
};


class MSPlane: public MSLandscape
{
private:
    int x = 0;
    int y = 0;
public:
    MSPlane(int x, int y) { this->x = x; this->y = y; }
    virtual ~MSPlane() {}
    float getHeightAt(const int, const int) { return 0.0f; }
    int getSizeX() { return x; }
    int getSizeY() { return y; }
};


class MSTexHeightmap: public MSLandscape
{
private:
    GLTexture* texture;
    float maxHeight = 1.0f;
public:
    MSTexHeightmap(GLTexture* texture) { this->texture = texture; }
    virtual ~MSTexHeightmap() {}
    float getHeightAt(const int x, const int y);
    int getSizeX() { return texture->getWidth(); }
    int getSizeY() { return texture->getHeight(); }
    GLTexture* getTexture() { return texture; }
    float getMaxHeight() { return maxHeight; }
    void setMaxHeight(float val) { maxHeight = val; }
};


class SNLandscapeNode: public ISceneNode
{
private:
    Pointer<MSLandscape> landscape;
    Vector<Vector3df> vertices;
    Vector<Vector3df> normals;
    Vector<Vector2df> texCoords;
    Vector<int> indices;
    
    GLfloat* bakedVertices = NULL;
    GLfloat* bakedNormals = NULL;
    GLfloat* bakedTexCoords = NULL;
    GLuint* bakedIndices = NULL;
    
    int lenVertices = 0;
    int lenIndices = 0;
protected:
    void render(GLContext* context);
public:
    SNLandscapeNode(const String& name, MSLandscape* landscape);
    virtual ~SNLandscapeNode();
    MSLandscape* getLandscape() { return landscape.get(); }
};


#endif /* sclandscapenode_hpp */
