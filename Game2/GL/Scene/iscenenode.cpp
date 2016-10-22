
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


/*void ISceneNode::enableLighting(const bool val)
{
    lightingEnabled = val;
    for (Pair<String, Pointer<ISceneNode>> child: *this) {
        child.second->enableLighting(val);
    }
}*/


void ISceneNode::draw(GLContext* context, bool alsoCamera) {
    context->pushMatrix(GLM_BOTH, false);
    glPushAttrib(GL_CURRENT_BIT);
    
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(scale.x, scale.y, scale.z);
    
    glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
    
    renderChildren(context);
    
    if (isLightEmitter()) {
        if (context->incLastLightIndex()) {
            setLightIndex(context->getLastLightIndex());
            render(context);
        }
    } else if (!isCamera() || alsoCamera) {
        if (texture != NULL) texture->Bind();
        
        /*GLuint colorBuffer = 0;
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_UNSIGNED_BYTE,
                     GL_RGB, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        GLuint frameBuffer = 0;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        
        render(context);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);*/
        
        render(context);
        
        if (texture != NULL) texture->Unbind();
    }
    
    glPopAttrib();
    context->popMatrix();
}
