#include "sncubenode.h"



void SNCubeNode::render(GLContext* context)
{
    GLfloat vertices[] = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 0, 0,
    };
    
    /*GLfloat texCoords[] = {
        (float) (src.x / w),           (float) (src.y / h),
        (float) ((src.x + src.w) / w), (float) (src.y / h),
        (float) ((src.x + src.w) / w), (float) ((src.y + src.h) / h),
        (float) (src.x / w),           (float) ((src.y + src.h) / h),
    };*/
    
    GLubyte indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    /*glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);*/
    
    //glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
