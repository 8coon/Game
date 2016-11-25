#include "sncameranode.h"


SNCameraNode::SNCameraNode(const String& name): ISceneNode(name)
{
}


Vector3df SNCameraNode::rayCast(float x, float y)
{
    Vector3df vec(x, y, -1.0f);
    
    Matrix4df modelView;
    GLfloat buffer[16];
    
    glGetFloatv(GL_MODELVIEW_MATRIX, buffer);
    x += 1;
    for (int i = 0; i < 16; i++) {
        modelView.data[i] = buffer[i];
    }
    
    modelView = modelView.inverse();
    vec = modelView.mul(vec);
    return vec;
}


void SNCameraNode::drawSceneRoot(GLContext *context, ISceneNode *root)
{
    this->rootNode = root;
    
    /*Deque<ISceneNode*> parents;
    ISceneNode* parent = this;
    
    while (parent != root) {
        parents.push_front(parent);
        parent = parent->getParent();
    }
    
    Deque<ISceneNode*>::iterator it = parents.begin();
    while (it != parents.end()) {
        //(*it)->pushTransformations(context, -1.0f);
        //std::cout << (*it)->getName() << std::endl;
        it++;
    }*/
    
    //std::cout << "===========" << std::endl;
    //this->render(context);
    this->draw(context, true);
    
    /*Deque<ISceneNode*>::reverse_iterator itr = parents.rbegin();
    while (itr != parents.rend()) {
        //(*itr)->popTransformations(context);
        itr++;
    }*/
}


void SNCameraNode::render(GLContext *context)
{
    /*gluLookAt(getXPos(), getYPos(), getZPos(),
              lookAt.x, lookAt.y, lookAt.z,
              0.0f, 1.0f, 0.0f);*/
    
    rootNode->draw(context);
}
