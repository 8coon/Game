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

#define SKIN_FILENAME "Scripts/skin.lua"


using namespace coon;

class UIWidget;
class UISkin;


class GLWindow
{
    LuaConfig* config;
    UISkin* skin;
    UIWidget* root;
    SDL_Window* window;
    SDL_Renderer* renderer;
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

    bool isRunning() { return running; }
    int getFPS() { return lastFPS; }
    SDL_Window* getWindow() { return window; }
    SDL_Renderer* getRenderer() { return renderer; }
    UIWidget* getRoot() { return root; }
    Point getResolution() { return resolution; }
};

#endif // GLWINDOW_H
