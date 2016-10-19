#include "mesh.h"


void Mesh::draw(const Vector3df& pos, const Vector3df& rot, const Vector3df& scale)
{
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glTranslatef(pos.x, pos.y, pos.z);
    
    glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
    
    glScalef(scale.x, scale.y, scale.z);
    
    glBegin(mode);
    
    bool useColors = !colors.empty();
    int i = 0;
    
    for (const Vector3df& v: vertices) {
        if (useColors) {
            glColor3f(colors[i].x, colors[i].y, colors[i].z);
            i++;
        }
        
        glVertex3f(v.x, v.y, v.z);
    }
    
    glEnd();
    glPopMatrix();
    glPopAttrib();
}



CubeMesh::CubeMesh(): Mesh(GL_QUADS)
{
    pushVertice(Vector3df( 1.0f,  1.0f, -1.0f), Vector3df(0.0f, 1.0f, 0.0f));
    pushVertice(Vector3df(-1.0f,  1.0f, -1.0f));
    pushVertice(Vector3df(-1.0f,  1.0f,  1.0f));
    pushVertice(Vector3df( 1.0f,  1.0f,  1.0f));
    
    pushVertice(Vector3df( 1.0f, -1.0f,  1.0f), Vector3df(1.0f, 0.5f, 0.0f));
    pushVertice(Vector3df(-1.0f, -1.0f,  1.0f));
    pushVertice(Vector3df(-1.0f, -1.0f, -1.0f));
    pushVertice(Vector3df( 1.0f, -1.0f, -1.0f));
    
    pushVertice(Vector3df( 1.0f,  1.0f,  1.0f), Vector3df(1.0f, 0.0f, 0.0f));
    pushVertice(Vector3df(-1.0f,  1.0f,  1.0f));
    pushVertice(Vector3df(-1.0f, -1.0f,  1.0f));
    pushVertice(Vector3df( 1.0f, -1.0f,  1.0f));
    
    pushVertice(Vector3df( 1.0f, -1.0f, -1.0f), Vector3df(1.0f, 1.0f, 0.0f));
    pushVertice(Vector3df(-1.0f, -1.0f, -1.0f));
    pushVertice(Vector3df(-1.0f,  1.0f, -1.0f));
    pushVertice(Vector3df( 1.0f,  1.0f, -1.0f));
    
    pushVertice(Vector3df(-1.0f,  1.0f,  1.0f), Vector3df(0.0f, 0.0f, 1.0f));
    pushVertice(Vector3df(-1.0f,  1.0f, -1.0f));
    pushVertice(Vector3df(-1.0f, -1.0f, -1.0f));
    pushVertice(Vector3df(-1.0f, -1.0f,  1.0f));
    
    pushVertice(Vector3df( 1.0f,  1.0f, -1.0f), Vector3df(1.0f, 0.0f, 0.0f));
    pushVertice(Vector3df( 1.0f,  1.0f,  1.0f));
    pushVertice(Vector3df( 1.0f, -1.0f,  1.0f));
    pushVertice(Vector3df( 1.0f, -1.0f, -1.0f));
}
