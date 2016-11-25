#ifndef UIFONT_H
#define UIFONT_H

#include "../../Strings/string.h"
#include "../../Strings/stringhash.h"
#include "../../Streams/streams.h"
#include "../../Streams/streamcollection.h"
#include "../../LuaWrapper/luawrapper.h"
#include "../../Containers/containers.h"
#include "uitexture.h"
#include "../../SDL2/SDL.h"


using namespace coon;


class UIFont;


class UIFont_setLetter: public LuaFunction
{
private:
    UIFont* font;
public:
    UIFont_setLetter(UIFont* font, LuaWrapper* w): LuaFunction(w) { this->font = font; }
    String getSignature() { return "string letter, number x, number y, number width, number height"; }
    void execute();
};


class UISurface;


class UIFont
{
private:
    SDL_Renderer* renderer;
    UITexture* texture;
    Map<String, Rect> letters;
    Map<String, RGBA> colors;
    RGBA defColor = RGBA(255, 255, 255, 255);
protected:
    void drawLetter(SDL_Renderer* renderer, const String& letter, Rect dst, RGBA color);
public:
    UIFont(SDL_Renderer* renderer, const String& fileName);
    ~UIFont() { delete texture; }

    UITexture* getTexture() { return texture; }
    void loadTexture(const String& fileName) { texture = new UITexture(renderer, fileName); }
    void setLetter(const String& letter, Rect rect) { letters[letter] = rect; }
    Rect getLetter(const String& letter) { return (letters.count(letter) > 0)?(letters[letter]):(letters["?"]); }
    RGBA getColor() { return defColor; }
    void setColor(RGBA color) { defColor = color; }
    void addColor(const String& code, RGBA color) { colors[code] = color; }
    RGBA getColor(const String& code) { return colors[code]; }
    bool hasColor(const String& code) { return colors.count(code) > 0; }
    Rect measureText(const String &text);
};

#endif // UIFONT_H
