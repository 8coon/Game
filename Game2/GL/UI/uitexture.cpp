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

    texture = SDL_CreateTextureFromSurface(renderer, loaded);

    if (texture == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl; return;
    }

    SDL_FreeSurface(loaded);
    SDL_QueryTexture(texture, &format, &access, &width, &height);
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

    texture = SDL_CreateTextureFromSurface(renderer, loaded);

    if (texture == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface error (from StreamCollection): " <<
                     SDL_GetError() << std::endl; return;
    }

    SDL_FreeSurface(loaded);
    SDL_QueryTexture(texture, &format, &access, &width, &height);
    delete[] buffer;
}


UITexture::~UITexture()
{
    if (texture != NULL) SDL_DestroyTexture(texture);
}
