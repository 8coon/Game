
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


void ISceneNode::pushTransformations(GLContext* context, float mult)
{
    context->pushMatrix(GLM_BOTH, false);
    glPushAttrib(GL_CURRENT_BIT);
    
    if (mult > 0.5f) {
        glTranslatef(pos.x * mult, pos.y * mult, pos.z * mult);
        glScalef(scale.x * mult, scale.y * mult, scale.z * mult);

        glRotatef(rot.x * mult, 1.0f, 0.0f, 0.0f);
        glRotatef(rot.y * mult, 0.0f, 1.0f, 0.0f);
        glRotatef(rot.z * mult, 0.0f, 0.0f, 1.0f);
    } else {
        glRotatef(rot.z * mult, 0.0f, 0.0f, 1.0f);
        glRotatef(rot.y * mult, 0.0f, 1.0f, 0.0f);
        glRotatef(rot.x * mult, 1.0f, 0.0f, 0.0f);
        
        glScalef(scale.x / mult, scale.y / mult, scale.z / mult);
        glTranslatef(pos.x * mult, pos.y * mult, pos.z * mult);
    }
}


void ISceneNode::popTransformations(GLContext *context)
{
    glPopAttrib();
    context->popMatrix();
}


void ISceneNode::draw(GLContext* context, bool alsoCamera) {
    pushTransformations(context);
    renderChildren(context);
    
    if (isLightEmitter()) {
        if (context->incLastLightIndex()) {
            setLightIndex(context->getLastLightIndex());
            render(context);
        }
    } else if (!isCamera() || alsoCamera) {
        int i = 0;
        
        for (GLTexture* texture: textures) {
            glActiveTexture(GL_TEXTURE0 + i);
            if (texture != NULL) texture->Bind();
            i++;
        }
        
        render(context);
        
        i = 0;
        for (GLTexture* texture: textures) {
            glActiveTexture(GL_TEXTURE0 + i);
            if (texture != NULL) texture->Unbind();
            i++;
        }
    }
    
    popTransformations(context);
}
