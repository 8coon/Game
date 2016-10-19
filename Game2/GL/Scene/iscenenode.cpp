
#include "iscenenode.h"


void ISceneNode::renderChildren(GLContext* context) {
    for (Pair<String, Pointer<ISceneNode>> child: *this) {
        child.second->render(context);
    }
}


void ISceneNode::draw(GLContext* context) {
    context->pushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    
    glTranslatef(pos.x, pos.y, pos.z);
    
    glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
    
    glScalef(scale.x, scale.y, scale.z);
    
    renderChildren(context);
    render(context);
    
    glPopAttrib();
    context->popMatrix();
}
