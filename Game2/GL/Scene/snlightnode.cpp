#include "snlightnode.h"


void SNLightNode::render(GLContext *context)
{
    GLenum light = GL_LIGHT0 + getLightIndex();
    
    if (!enabled) {
        glDisable(light);
        return;
    }
    
    glEnable(light);
        
    GLfloat floats[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(light, GL_EMISSION, floats);
        
    RGBA2GLfloatv(diffuse, floats, 4);
    glLightfv(light, GL_DIFFUSE, floats);
        
    RGBA2GLfloatv(specular, floats, 4);
    glLightfv(light, GL_SPECULAR, floats);
    
    GLfloat pos[] = { 0.0, 0.0, 0.0, 0.0 };
    glLightfv(light, GL_POSITION, pos);
}
