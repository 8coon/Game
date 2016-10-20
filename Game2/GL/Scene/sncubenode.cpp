#include "sncubenode.h"



void SNCubeNode::render(GLContext* context)
{
    GLfloat vertices[] = {
        // Front side
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        
        // Right side
         0.5, -0.5,  0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
         0.5,  0.5,  0.5,
        
        // Left side
        -0.5, -0.5,  0.5,
        -0.5, -0.5, -0.5,
        -0.5,  0.5, -0.5,
        -0.5,  0.5,  0.5,
        
        // Back side
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
        
        // Bottom side
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,
        
        // Top side
        -0.5,  0.5,  0.5,
         0.5,  0.5,  0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
    };
    
    GLfloat texCoords[] = {
        // Front side
        0, 1,
        1, 1,
        1, 0,
        0, 0,
        
        // Right side
        0, 1,
        1, 1,
        1, 0,
        0, 0,
        
        // Left side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
        
        // Back side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
        
        // Bottom side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
        
        // Top side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
    };
    
    GLubyte indices[] = {
        // Front size
        0,  1,  2,
        0,  2,  3,
        
        // Right side
        4,  5,  6,
        4,  6,  7,
        
        // Left side
        8,  9,  10,
        8,  10, 11,
        
        // Back side
        12, 13, 14,
        12, 14, 15,
        
        // Bottom side
        16, 17, 18,
        16, 18, 19,
        
        // Top side
        20, 21, 22,
        20, 22, 23,
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    
    //glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_BYTE, indices);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
