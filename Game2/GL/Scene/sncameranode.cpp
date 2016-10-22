#include "sncameranode.h"



void SNCameraNode::drawSceneRoot(GLContext *context, ISceneNode *root)
{
    this->rootNode = root;
    this->draw(context, true);
}


void SNCameraNode::render(GLContext *context)
{
    /*gluLookAt(getXPos(), getYPos(), getZPos(),
              lookAt.x, lookAt.y, lookAt.z,
              0.0f, 1.0f, 0.0f);*/
    
    rootNode->draw(context);
}
