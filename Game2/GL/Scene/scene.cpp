#include "scene.h"



void Scene::draw(GLContext* context)
{
    if (glIsEnabled(GL_LIGHTING)) {
        GLfloat color[] = { 0.0, 0.0, 0.0, 0.0 };
        RGBA2GLfloatv(ambientColor, color, 4);
        
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
        glEnable(GL_COLOR_MATERIAL);
        glColor4f(color[0], color[1], color[2], color[3]);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
    }
    
    //setLighting(true);
    
    //root->draw(context);
    //setLighting(false);
    
    activeCamera->drawSceneRoot(context, root.get());
}


Scene::Scene()
{
    root = Pointer<ISceneNode>(new ISceneNode("root"));
    activeCamera = new SNCameraNode("camera");
    root->addChild(activeCamera);
}


void Scene::setLighting(bool val)
{
    if (val) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
}
