#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "../SDL2/SDL.h"
#include "../SDL2_image/SDL_image.h"
#include <iostream>
#include "../Strings/string.h"
#include "../LuaWrapper/luaconfig.h"
#include "../common.h"
#include "../GL/UI/uitexture.h"
#include "../GL/UI/uiskin.h"
#include "../GL/UI/Widgets/uiwidget.h"
#include "glcontext.h"

#define SKIN_FILENAME "Scripts/skin.lua"


using namespace coon;

class UIWidget;
class UISkin;


class GLContext;


class GLWindow
{
    LuaConfig* config;
    UISkin* skin;
    UIWidget* root;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* renderTexture;
    GLContext* context;
    int lastTime;
    int frames = 0;
    int lastFPSTime = 0;
    int lastFPS = 0;
    bool running = true;
    Point resolution;
public:
    GLWindow(LuaConfig* config);
    ~GLWindow();

    void processEvents();
    void startFrame();
    void endFrame();

    void drawUI();

    void drawTexture(UITexture* texture, Rect src, Rect dst);
    void drawTexture(UITexture* texture, Rect src, int x, int y);
    void drawTexture(UITexture* texture, int x, int y);
    void drawTexture(GLuint texture, Rect src, Rect dst, float w, float h);

    bool isRunning() { return running; }
    int getFPS() { return lastFPS; }
    SDL_Window* getWindow() { return window; }
    SDL_Renderer* getRenderer() { return renderer; }
    //SDL_Texture* getRenderTexture() { return renderTexture; }
    UIWidget* getRoot() { return root; }
    Point getResolution() { return resolution; }
    GLContext* getContext() { return context; }
};

#endif // GLWINDOW_H
