#ifndef UISKIN_H
#define UISKIN_H

#include "../../SDL2/SDL.h"
#include "uitexture.h"
#include "uiwidgettexture.h"
#include "../../Containers/containers.h"
#include "../../Strings/string.h"
#include "../../Strings/stringhash.h"
#include "../../LuaWrapper/luawrapper.h"
#include "../../GL/UI/uitexture.h"
#include "uifont.h"

using namespace coon;


class UISkin;


class UISkin_loadTexture: public LuaFunction
{
private:
    UISkin* skin;
public:
    UISkin_loadTexture(UISkin* skin, LuaWrapper* w): LuaFunction(w) { this->skin = skin; }
    String getSignature() { return "string fileName"; }
    void execute();
};


class UISkin_setWidgetTexture: public LuaFunction
{
private:
    UISkin* skin;
public:
    UISkin_setWidgetTexture(UISkin* skin, LuaWrapper* w): LuaFunction(w) { this->skin = skin; }
    String getSignature() { return "string name, number l, number t, number h, number w, number left, number top"; }
    void execute();
};


class UISkin_loadFont: public LuaFunction
{
private:
    UISkin* skin;
public:
    UISkin_loadFont(UISkin* skin, LuaWrapper* w): LuaFunction(w) { this->skin = skin; }
    String getSignature() { return "string fileName"; }
    void execute();
};


class UIFont;


class UISkin
{
private:
    UITexture* texture;
    UIFont* font;
    Map<String, UIWidgetTexture> widgets;
    SDL_Renderer* renderer;
public:
    UISkin(SDL_Renderer* renderer, const String& fileName);
    ~UISkin() { delete texture; }

    void loadTexture(const String& fileName) { texture = new UITexture(renderer, fileName); }
    void loadFont(const String& fileName) { font = new UIFont(renderer, fileName); }
    void setWidgetTexture(const String& name, UIWidgetTexture texture) { widgets[name] = texture; }

    UIWidgetTexture getWidgetTexture(const String& name);

    UITexture* getTexture() { return texture; }
    UIFont* getFont() { return font; }
};

#endif // UISKIN_H
