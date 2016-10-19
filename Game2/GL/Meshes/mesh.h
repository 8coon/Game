#ifndef mesh_hpp
#define mesh_hpp

#include <stdio.h>
#include "../../Containers/containers.h"
#include "../../Strings/string.h"
#include "../../common.h"

#include "../../SDL2/SDL.h"
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


using namespace coon;


class Mesh
{
private:
    Vector<Vector3df> vertices;
    Vector<Vector3df> colors;
    
    int mode;
protected:
    Vector<Vector3df>& getVertices() { return vertices; }
    Vector<Vector3df>& getColors() { return colors; }
    
    void pushVertice(const Vector3df& vec, const Vector3df& color)
            { vertices.push_back(vec); colors.push_back(color); }
    void pushVertice(const Vector3df& vec)
            { vertices.push_back(vec); if (!colors.empty()) {
                Vector3df v = colors.back(); colors.push_back(v); }}
public:
    Mesh(int mode = GL_TRIANGLES /* GL_QUADS */) { this->mode = mode; }
    Mesh(const Mesh& mesh) = delete;
    
    int getMode() { return mode; }
    
    virtual void draw(const Vector3df& pos, const Vector3df& rot,
                      const Vector3df& scale);
    void draw(const Vector3df& pos, const Vector3df& rot)
            { draw(pos, rot, Vector3df(1.0f, 1.0f, 1.0f)); }
    void draw(const Vector3df& pos)
            { draw(pos, Vector3df(0.0f, 0.0f, 0.0f)); }
    void draw()
            { draw(Vector3df(0.0f, 0.0f, 0.0f)); }
    
    // virtual void setTexture(int index, void* texture) = 0;
};


class CubeMesh: public Mesh
{
public:
    CubeMesh();
};




#endif /* mesh_h */
