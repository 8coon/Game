
#include "iscenenode.h"


void ISceneNode::renderChildren(GLContext* context) {
    for (Pair<String, Pointer<ISceneNode>> child: *this) {
        child.second->draw(context);
    }
}


ISceneNode* ISceneNode::findNodeByName(const String& name)
{
    if (this->name == name) return this;
    
    for (Pair<String, Pointer<ISceneNode>> child: *this) {
        ISceneNode* found = child.second->findNodeByName(name);
        if (found != NULL) return found;
    }
    
    return NULL;
}


void ISceneNode::draw(GLContext* context) {
    context->pushMatrix(GLM_BOTH, false);
    glPushAttrib(GL_CURRENT_BIT);
    
    glTranslatef(pos.x, pos.y, pos.z);
    
    glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
    
    glScalef(scale.x, scale.y, scale.z);
    
    renderChildren(context);
    
    if (texture != NULL) SDL_GL_BindTexture(texture->getTexture(), NULL, NULL);
    render(context);
    if (texture != NULL) SDL_GL_UnbindTexture(texture->getTexture());
    
    glPopAttrib();
    context->popMatrix();
}
