#include "glwindow.h"

GLWindow::GLWindow(LuaConfig *config)
{
    this->config = config;
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
        
        if (config->getFullscreen()) flags = flags | SDL_WINDOW_FULLSCREEN;
        if (config->getHighDPI()) flags = flags | SDL_WINDOW_ALLOW_HIGHDPI;
        
        resolution.x = config->getResolution().x;
        resolution.y = config->getResolution().y;

        window = SDL_CreateWindow(PROJECT_TITLE, 100, 100,
                                  config->getResolution().x, config->getResolution().y,
                                  flags);
        if (window == nullptr) {
            std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl; return;
        }

        flags = SDL_RENDERER_ACCELERATED;
        if (config->getVSync()) flags = flags | SDL_RENDERER_PRESENTVSYNC;
        renderer = SDL_CreateRenderer(window, -1, flags);

        if (renderer == nullptr) {
            std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl; return;
        }

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        flags = IMG_INIT_PNG;
        if (!(IMG_Init(flags) & flags)) {
            std::cout << "IMG_Init error: " << IMG_GetError() << std::endl; return;
        }

        skin = new UISkin(renderer, SKIN_FILENAME);
        root = new UIWidget();
        root->setSkin(skin);
        root->setLayout(new UIAdjustiveLayout());
    } else {
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
    }
}


GLWindow::~GLWindow()
{
    delete root;
    delete skin;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void GLWindow::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        /*int realw = 0, realh = 0;
        int imw = 0, imh = 0;
        
        SDL_GetWindowSize(this->getWindow(), &imw, &imh);
        SDL_GL_GetDrawableSize(this->getWindow(), &realw, &realh);
        
        double ratiow = (realw * 1.0) / imw;
        double ratioh = (realh * 1.0) / imh;*/
        
        //if (config->getHighDPI()) {
        /*event.motion.x *= ratiow;
        event.motion.y *= ratioh;
        event.wheel.y *= ratioh;*/
        //}
        
        
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEMOTION: {
                Point pos(event.motion.x, event.motion.y);
                UIMouseEvent ev(pos, pos);
                getRoot()->mouseEvent(ev);
                } break;

            case SDL_MOUSEBUTTONDOWN: {
                Point pos(event.motion.x, event.motion.y);
                UIMouseEvent ev(pos, pos);

                switch (event.button.button) {
                    case SDL_BUTTON_LEFT: ev.button = UI_BUTTON_LEFT; break;
                    case SDL_BUTTON_MIDDLE: ev.button = UI_BUTTON_MIDDLE; break;
                    case SDL_BUTTON_RIGHT: ev.button = UI_BUTTON_RIGHT; break;
                }

                ev.type = UI_MOUSE_PRESSED;
                getRoot()->mouseEvent(ev);
            } break;

            case SDL_MOUSEBUTTONUP: {
                Point pos(event.motion.x, event.motion.y);
                UIMouseEvent ev(pos, pos);

                switch (event.button.button) {
                    case SDL_BUTTON_LEFT: ev.button = UI_BUTTON_LEFT; break;
                    case SDL_BUTTON_MIDDLE: ev.button = UI_BUTTON_MIDDLE; break;
                    case SDL_BUTTON_RIGHT: ev.button = UI_BUTTON_RIGHT; break;
                }

                ev.type = UI_MOUSE_RELEASED;
                getRoot()->mouseEvent(ev);
            } break;

            case SDL_MOUSEWHEEL: {
                int x = 0; int y = 0;
                SDL_GetMouseState(&x, &y);
                Point pos(x, y);
                UIMouseEvent ev(pos, pos);
                ev.scroll = event.wheel.y;
                ev.type = UI_MOUSE_SCROLL;
                getRoot()->mouseEvent(ev);
            } break;

            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        UIResizeEvent ev;
                        ev.height = event.window.data2;
                        ev.width = event.window.data1;
                        getRoot()->resizeEvent(ev);
                    } break;
                }
            } break;
        }
    }
}


void GLWindow::startFrame()
{
    SDL_RenderClear(renderer);
    lastTime = SDL_GetTicks();
}


void GLWindow::endFrame()
{
    int time = SDL_GetTicks();
    int delta = abs(time - lastTime);
    lastTime = time;

    double ideal = 1000.0f / config->getFPS();
    delta = ideal - delta;
    if (delta < 0) delta = 1;

    frames++;
    if (time - lastFPSTime >= 1000) {
        lastFPS = frames;
        lastFPSTime = time;
        frames = 0;

        StringBuilder builder;
        builder.append(PROJECT_TITLE);
        builder.append(" [");
        builder.append(PROJECT_VERSION);
        builder.append("] - FPS: ");
        builder.append(String::fromInt(lastFPS));

        SDL_SetWindowTitle(window, builder.toString().toStdString().c_str());
    }

    SDL_RenderSetClipRect(getRenderer(), NULL);
    //SDL_RenderSetViewport(getRenderer(), NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(delta);
}


void GLWindow::drawUI()
{
    UIWindowSurface surface(this);
    getRoot()->paintEvent(&surface);
    surface.clearClipping();
}


void GLWindow::drawTexture(UITexture *texture, Rect src, Rect dst)
{
    SDL_Rect _src; RECT2SDL(_src, src);
    SDL_Rect _dst; RECT2SDL(_dst, dst);
    SDL_RenderCopy(renderer, texture->getTexture(), &_src, &_dst);
}


void GLWindow::drawTexture(UITexture *texture, Rect src, int x, int y)
{
    drawTexture(texture, src, Rect(x, y, texture->getWidth(), texture->getHeight()));
}


void GLWindow::drawTexture(UITexture *texture, int x, int y)
{
    drawTexture(texture, Rect(0, 0, texture->getWidth(), texture->getHeight()), x, y);
}

