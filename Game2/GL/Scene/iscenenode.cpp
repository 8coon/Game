
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
        int i = 0;
        
        //std::cout << "Textures: " << textures.size() << std::endl;
        for (GLTexture* texture: textures) {
            glActiveTexture(GL_TEXTURE0 + i);
            //std::cout << "Binding " << i << ", " << (void*) texture << std::endl;
            if (texture != NULL) texture->Bind();
            i++;
        }
        
        //glActiveTexture(GL_TEXTURE0);
        render(context);
        
        i = 0;
        for (GLTexture* texture: textures) {
            glActiveTexture(GL_TEXTURE0 + i);
            if (texture != NULL) texture->Unbind();
            i++;
        }
    }
    
    glPopAttrib();
    context->popMatrix();
}
