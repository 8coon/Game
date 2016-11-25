#ifndef uiscene_hpp
#define uiscene_hpp

#include "../../../common.h"
#include "../../../Containers/containers.h"
#include "../../../Strings/string.h"
#include "uiwidget.h"
#include "../../Scene/scene.h"
#include "../../glcontext.h"
#include "uiactionbinder.h"


class UIActionBinder;


class UIScene: public UIWidget
{
private:
    Pointer<Scene> scene;
    Pointer<UIActionBinder> binder;
    GLContext* context;
protected:
    void setupEvents();
public:
    UIScene(UIWidget* parent): UIWidget(parent)
            { scene = Pointer<Scene>(new Scene());
              binder = Pointer<UIActionBinder>(new UIActionBinder(this));
              setupEvents();
              this->setHeight(parent->getHeight());
              this->setWidth(parent->getWidth());
              this->setLeft(0);
              this->setTop(0);
            }
    virtual ~UIScene() {}
    
    bool paintEvent(UISurface *surface);
    void setContext(GLContext* context) { this->context = context; }
    GLContext* getContext() { return context; }
    Scene* getScene() { return scene.get(); }
    UIActionBinder* getActionBinder() { return binder.get(); }
};


#endif /* uiscene_hpp */
