#ifndef gltexture_hpp
#define gltexture_hpp

#include <stdio.h>


#include "UI/uitexture.h"
#include "../Containers/containers.h"
#include "../Strings/string.h"
#include "glwindow.h"

#include "../SDL2/SDL.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


using namespace coon;


class GLTexture: public UITexture
{
private:
    RGBA diffuse = RGBA(255, 255, 255, 255);
    RGBA specular = RGBA(255, 255, 255, 255);
    RGBA emission = RGBA(0, 0, 0, 0);
    bool hasMipmaps = false;
protected:
    void generateMipmaps();
public:
    GLTexture(SDL_Texture* texture = NULL): UITexture(texture) {}
    GLTexture(SDL_Renderer* renderer, const String& fileName)
        : UITexture(renderer, fileName) {}
    GLTexture(SDL_Renderer* renderer, StreamCollection* collection,
              const String &name): UITexture(renderer, collection, name) {}
    virtual ~GLTexture() {};
    
    void setMipmapsEnabled(bool val) { hasMipmaps = val; }
    bool areMipmapsEnabled() { return hasMipmaps; }
    
    void setDiffuse(const RGBA& color) { diffuse = color; }
    void setSpecular(const RGBA& color) { specular = color; }
    void setEmission(const RGBA& color) { emission = color; }
    RGBA getDiffuse() { return diffuse; }
    RGBA getSpecular() { return specular; }
    RGBA getEmission() { return emission; }
    
    void Bind();
    void Unbind();
};


#endif /* gltexture_hpp */
