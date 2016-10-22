#include "uitexture.h"

UITexture::UITexture(SDL_Texture* texture)
{
    this->texture = texture;
    if (texture != NULL)
        SDL_QueryTexture(texture, &format, &access, &width, &height);
}


UITexture::UITexture(SDL_Renderer* renderer, const String& fileName)
{
    SDL_Surface* loaded = IMG_Load(fileName.toStdString().c_str());

    if (loaded == nullptr) {
        std::cout << "IMG_Load error: " << IMG_GetError() << std::endl; return;
    }

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA8888, 0);
    texture = SDL_CreateTextureFromSurface(renderer, converted);

    if (texture == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl; return;
    }

    SDL_QueryTexture(texture, &format, &access, &width, &height);
    getRawFromSurface(converted);
    SDL_FreeSurface(loaded);
    SDL_FreeSurface(converted);
}


UITexture::UITexture(SDL_Renderer *renderer, StreamCollection *collection, const String &name)
{
    int len = collection->getRecordLen(name);
    char* buffer = new char[len];
    collection->fillBuffer(name, buffer, len);

    SDL_RWops* rw = SDL_RWFromMem(buffer, len);
    SDL_Surface* loaded = IMG_LoadPNG_RW(rw);
    SDL_RWclose(rw);

    if (loaded == nullptr) {
        std::cout << "IMG_Load (from StreamCollection) error: " << IMG_GetError() << std::endl; return;
    }

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA8888, 0);
    texture = SDL_CreateTextureFromSurface(renderer, converted);

    if (texture == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface error (from StreamCollection): " <<
                     SDL_GetError() << std::endl; return;
    }

    SDL_QueryTexture(texture, &format, &access, &width, &height);
    getRawFromSurface(converted);
    SDL_FreeSurface(loaded);
    SDL_FreeSurface(converted);
    delete[] buffer;
}


void UITexture::getRawFromSurface(SDL_Surface *surface)
{
    rawData = new RGBA32[surface->w * surface->h];
    
    for (int i = 0; i < surface->w * surface->h; i++) {
        rawData[i] = RGBA32(((uint32_t*)(surface->pixels))[i]);
    }
}


UITexture::~UITexture()
{
    if (texture != NULL) SDL_DestroyTexture(texture);
    setRawData(NULL);
}
