#ifndef UITEXTURE_H
#define UITEXTURE_H

#include "../../common.h"
#include "../../Streams/streams.h"
#include "../../Strings/string.h"
#include "../../Streams/streamcollection.h"

#include "../../SDL2/SDL.h"
#include "../../SDL2_image/SDL_image.h"


using namespace coon;


class UITexture
{
private:
    SDL_Texture* texture;
    int width;
    int height;
    int access;
    unsigned int format;
    RGBA32* rawData = NULL;
protected:
    void getRawFromSurface(SDL_Surface* surface);
public:
    UITexture(SDL_Texture* texture = NULL);
    UITexture(SDL_Renderer* renderer, const String& fileName);
    UITexture(SDL_Renderer* renderer, StreamCollection* collection, const String &name);
    virtual ~UITexture();

    SDL_Texture* getTexture() { return texture; }
    int getFormat() { return format; }
    int getAccess() { return access; }
    int getHeight() { return height; }
    int getWidth() { return width; }
    void setTextureUnsafe(SDL_Texture* texture) { this->texture = texture; }
    RGBA32* getRawData() { return rawData; }
    void setRawData(RGBA32* data)
            { if (rawData != NULL) delete[] rawData; rawData = data; }
};

#endif // UITEXTURE_H
