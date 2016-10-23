#include "gltexture.h"



void GLTexture::Bind()
{
    if (glIsEnabled(GL_LIGHTING)) {
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(diffuse.r / 255.0, diffuse.g / 255.0, diffuse.b / 255.0);
    
        glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(specular.r / 255.0, specular.g / 255.0, specular.b / 255.0);
    
        glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(emission.r / 255.0, emission.g / 255.0, emission.b / 255.0);
    }

    SDL_GL_BindTexture(getTexture(), NULL, NULL);
}


void GLTexture::Unbind()
{
    if (hasMipmaps) {
        //generateMipmaps();
    }
    
    if (glIsEnabled(GL_LIGHTING)) {
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(1.0, 1.0, 1.0);
    
        glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(1.0, 1.0, 1.0);
    
        glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(1.0, 1.0, 1.0);
    }
    
    SDL_GL_UnbindTexture(getTexture());
}


void GLTexture::generateMipmaps()
{
    SDL_GL_BindTexture(this->getTexture(), NULL, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_GL_UnbindTexture(this->getTexture());
}




