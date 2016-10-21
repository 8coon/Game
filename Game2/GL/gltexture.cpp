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


