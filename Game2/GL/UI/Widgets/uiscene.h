#ifndef uiscene_hpp
#define uiscene_hpp

#include "../../../common.h"
#include "../../../Containers/containers.h"
#include "../../../Strings/string.h"
#include "uiwidget.h"
#include "../../Scene/scene.h"
#include "../../glcontext.h"


class UIScene: public UIWidget
{
private:
    Pointer<Scene> scene;
    GLContext* context;
public:
    UIScene(UIWidget* parent): UIWidget(parent)
            { scene = Pointer<Scene>(new Scene()); }
    virtual ~UIScene() {}
    
    bool paintEvent(UISurface *surface);
    void setContext(GLContext* context) { this->context = context; }
    GLContext* getContext() { return context; }
    Scene* getScene() { return scene.get(); }
};


#endif /* uiscene_hpp */
